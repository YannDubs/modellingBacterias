/*
 * prjsv 2015
 * 2013, 2014
 * Marco Antognini
 */

#include <Application.hpp>
#include <Config.hpp>
#include <JSON/JSONSerialiser.hpp>
#include <Utility/Constants.hpp>

#include <iostream>
#include <cassert>

namespace
{

Application* currentApp = nullptr; ///< Current application

std::string applicationDirectory(int argc, char const** argv)
{
    assert(argc >= 1);

    auto dir = std::string(argv[0]);

    auto lastSlashPos = dir.rfind('/');
    if (lastSlashPos == std::string::npos) {
        dir = "./";
    } else {
        dir = dir.substr(0, lastSlashPos + 1);
    }

    return dir;
}

std::string configFileRelativePath(int argc, char const** argv)
{
    if (argc >= 2) {
        return RES_LOCATION + argv[1];
    } else {
        return RES_LOCATION + DEFAULT_CFG;
    }
}

Vec2d getWindowSize()
{
    auto cfg = getAppConfig()["window"];
    auto width = cfg["simulation"]["size"].toDouble() + cfg["settings"]["width"].toDouble();
    auto height = cfg["simulation"]["size"].toDouble() + cfg["stats"]["height"].toDouble();
    return { width, height };
}

Vec2d getStatsSize()
{
    auto cfg = getAppConfig()["window"];
    auto width = cfg["simulation"]["size"].toDouble() + cfg["settings"]["width"].toDouble();
    auto height = cfg["stats"]["height"].toDouble();
    return { width, height };
}

Vec2d getSimulationSize()
{
    auto cfg = getAppConfig()["window"];
    auto size = cfg["simulation"]["size"].toDouble();
    return { size, size };
}

Vec2d getSettingsSize()
{
    auto cfg = getAppConfig()["window"];
    auto width = cfg["settings"]["width"].toDouble();
    auto height = cfg["simulation"]["size"].toDouble();
    return { width, height };
}

sf::View setupView(Vec2d size, sfg::Widget::Ptr widget, sf::Vector2u windowSize)
{
    auto position = widget->GetAbsolutePosition();
    auto alloc = widget->GetAllocation();

    sf::View view;
    view.reset({ { 0, 0 }, size });
    view.setViewport({ position.x / windowSize.x, position.y / windowSize.y, alloc.width / windowSize.x,
                       alloc.height / windowSize.y });

    return view;
}

} // anonymous

Application::Application(int argc, char const** argv)
: mAppDirectory(applicationDirectory(argc, argv))
, mCfgFile(configFileRelativePath(argc, argv))
, mConfig(j::readFromFile(mAppDirectory + mCfgFile))
, mPaused(false)
{
    // Set global singleton
    assert(currentApp == nullptr);
    currentApp = this;

    // Load the font
    if (!mFont.loadFromFile(mAppDirectory + FONT_LOCATION)) {
        std::cerr << "Couldn't load " << FONT_LOCATION << std::endl;
    }

    // Load lab and stats
    mEnv = new Lab;
	// A DECOMMENTER APRES LE CODAGE DE LA CLASSE Stats
    mStats = new Stats;
}

Application::~Application()
{
    // Destroy lab and stats, in reverse order
	// A DECOMMENTER APRES LE CODAGE DE LA CLASSE Stats
    delete mStats;
    delete mEnv;

    // Release textures
    for (auto& kv : mTextures) {
        delete kv.second;
        kv.second = nullptr;
    }
    mTextures.clear();

    // Reset the global pointer
    currentApp = nullptr;
}

void Application::run()
{
    // Create the GUI
    auto guiWindow = createGUI();
    sfg::Desktop desktop;
    desktop.Add(guiWindow);

    // Set up subclasses
    onRun();
    onSimulationStart();

    // Create the SFML window
    createWindow({ guiWindow->GetAllocation().width, guiWindow->GetAllocation().height });

    // Views for rendering regions
    createViews();

    // Load simulation background
    auto simulationBackground = sf::RectangleShape(getWorldSize());
    simulationBackground.setPosition(0, 0);
    auto backgroundId = getConfig()["simulation"]["background"].toString();
    simulationBackground.setTexture(&getAppTexture(backgroundId));

    // Create the Stats background (white board)
    auto statsBackground = sf::RectangleShape();
    statsBackground.setSize(getStatsSize());
    statsBackground.setFillColor(sf::Color::Black);

    // Use a clock to track time
    sf::Clock clk;

    // Main loop
    while (mRenderWindow.isOpen()) {
        // Handle events
        sf::Event event;
        while (mRenderWindow.pollEvent(event)) {
            desktop.HandleEvent(event);
            handleEvent(event, mRenderWindow);
        }

        // Set active graph
		
		  // A DECOMMENTER APRES LE CODAGE DE LA CLASSE Stats
        auto activeIndex = mStatsTabs->GetCurrentPage();
        getStats().setActive(activeIndex);
        
        // Update logics
        float const timeFactor = getConfig()["simulation"]["time factor"].toDouble();
        sf::Time dt = clk.restart() * timeFactor; // Always reset the clock!
        if (!mPaused) {
            getEnv().update(dt);
			// A DECOMMENTER APRES LE CODAGDE DE LA CLASSE Stats
            getStats().update(dt);
            onUpdate(dt);
        }

        // Update UI and render everything
        desktop.Update(dt.asSeconds());
        render(simulationBackground, statsBackground);
    }
}

Lab& Application::getEnv()
{
    return *mEnv;
}

Lab const& Application::getEnv() const
{
    return *mEnv;
}

j::Value& Application::getConfig()
{
    return mConfig;
}

j::Value const& Application::getConfig() const
{
    return mConfig;
}

sf::Font const& Application::getFont() const
{
    return mFont;
}

sf::Texture& Application::getTexture(std::string const& name)
{
    auto const it = mTextures.find(name);
    if (it != mTextures.end())
        return *it->second;
    else {
        // The texture is not yet in memory so we load it now
        sf::Texture* newTexture = new sf::Texture;
        if (newTexture->loadFromFile(getResPath() + name)) {
            // The texture was correctly loaded so we save it
            mTextures[name] = newTexture;
            // And return the texture
            return *newTexture;
        } else {
            // The file was not loaded correctly so we clean up the mess
            delete newTexture;
            newTexture = nullptr;
            // And return the default texture
            return mDefaultTexture;
        }
    }
}

std::string Application::getResPath() const
{
    return mAppDirectory + RES_LOCATION;
}

Vec2d Application::getWorldSize() const
{
    auto size = getConfig()["world"]["size"].toDouble();
    return { size, size };
}

Vec2d Application::getCentre() const
{
    return getWorldSize() / 2.0;
}

void Application::onRun()
{
    // Add save button
    auto saveButton = sfg::Button::Create("save configuration");
    saveButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&Application::saveConfig, this));
    packIntoLeftMenu("General", saveButton);

    // Add import/export buttons
    auto loadLabel = sfg::Label::Create("map.json");
    auto importButton = sfg::Button::Create("import");
    auto exportButton = sfg::Button::Create("export");
    auto loaderBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 10);
    loaderBox->Pack(loadLabel);
    loaderBox->Pack(importButton);
    loaderBox->Pack(exportButton);
    packIntoLeftMenu("General", loaderBox);
    //importButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&Application::importLab, this));
    //exportButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&Application::exportLab, this));

    // Add pause checkbox
    auto pauseCheckbox = sfg::CheckButton::Create("pause");
    pauseCheckbox->SetActive(mPaused);
    pauseCheckbox->GetSignal(sfg::ToggleButton::OnToggle)
        .Connect(std::bind(&Application::togglePause, this, pauseCheckbox));
    packIntoLeftMenu("General", pauseCheckbox);

    // Add debug checkbox
    packIntoLeftMenuNewCheckbox("General", "debug", { "debug" });

    // Add time factor
    packIntoLeftMenuNewSlider("General", "time factor", { "simulation", "time factor" }, 0.25, 4, 0.25);

    // Dish factors
    //CHANGER : temperature max 200
    packIntoLeftMenuNewSlider("General", "dish temperature", { "petri dish", "temperature" }, 0, 200, 5);
    packIntoLeftMenuNewSlider("General", "gradient exponent", { "petri dish", "gradient", "exponent" }, 0, 10, 0.1);

    // Generator factors
    packIntoLeftMenuNewSlider("Generator", "nutriment delay", { "generator", "nutriment", "delay" }, 0.1, 10, 0.1);
    packIntoLeftMenuNewSlider("Generator", "nutriment A/B balance", { "generator", "nutriment", "prob" }, 0, 1, 0.05);

    // Nutriment A factors
    packIntoLeftMenuNewSlider("Nutriment A", "growth min temp", { "nutriments", "A", "growth", "min temperature" }, 0,
                              200, 5);
    packIntoLeftMenuNewSlider("Nutriment A", "growth max temp", { "nutriments", "A", "growth", "max temperature" }, 0,
                              200, 5);
    packIntoLeftMenuNewSlider("Nutriment A", "growth speed", { "nutriments", "A", "growth", "speed" }, 0, 50, 5);
    packIntoLeftMenuNewSlider("Nutriment A", "min quantity", { "nutriments", "A", "quantity", "min" }, 1, 50, 1);
    packIntoLeftMenuNewSlider("Nutriment A", "max quantity", { "nutriments", "A", "quantity", "max" }, 1, 50, 1);

    // Nutriment B factors
    packIntoLeftMenuNewSlider("Nutriment B", "growth min temp", { "nutriments", "B", "growth", "min temperature" }, 0,
                              200, 5);
    packIntoLeftMenuNewSlider("Nutriment B", "growth max temp", { "nutriments", "B", "growth", "max temperature" }, 0,
                              200, 5);
    packIntoLeftMenuNewSlider("Nutriment B", "growth speed", { "nutriments", "B", "growth", "speed" }, 0, 50, 5);
    packIntoLeftMenuNewSlider("Nutriment B", "min quantity", { "nutriments", "B", "quantity", "min" }, 1, 50, 1);
    packIntoLeftMenuNewSlider("Nutriment B", "max quantity", { "nutriments", "B", "quantity", "max" }, 1, 50, 1);
    packIntoLeftMenuNewSlider("Nutriment B", "nutritive factor", { "nutriments", "B", "nutritive factor" }, 0, 100, 1);
    packIntoLeftMenuNewSlider("Nutriment B", "poison factor", { "nutriments", "B", "poison factor" }, 0, 100, 1);
    packIntoLeftMenuNewSlider("Nutriment B", "resistance factor", { "nutriments", "B", "resistance factor" }, 0, 100,
                              1);

    // Setup stats
    //CHANGER: nous trouvons intéressant de pouvoir comparer avec la température
    addGraph(s::GENERAL, { s::SIMPLE_BACTERIA, s::TWITCHING_BACTERIA, s::SWARM_BACTERIA, s::PLASMID_BACTERIA, s::NUTRIMENT_SOURCES, s::DISH_TEMPERATURE }, 0, 200);

	//addGraph(s::GENERAL, { s::SIMPLE_BACTERIA, s::TWITCHING_BACTERIA, s::SWARM_BACTERIA, s::NUTRIMENT_SOURCES }, 0, 200);
    addGraph(s::NUTRIMENT_QUANTITY, { s::NUTRIMENT_QUANTITY }, 0, 20000);
	/*
    addGraph(s::BACTERIA, { s::SPEED }, 20, 50);
    addGraph(s::SIMPLE_BACTERIA, { s::BETTER, s::WORSE }, 0, 10);
    addGraph(s::TWITCHING_BACTERIA, { s::TENTACLE_LENGTH, s::TENTACLE_SPEED }, 0, 300);
	*/
}

void Application::onEvent(sf::Event, sf::RenderWindow&)
{
    // By default nothing is done here
}

void Application::onSimulationStart()
{
    // By default nothing is done here
}

void Application::onUpdate(sf::Time)
{
    // By default nothing is done here
}

void Application::onDraw(sf::RenderTarget&)
{
    // By default nothing is done here
}

Vec2d Application::getCursorPositionInView() const
{
    return mRenderWindow.mapPixelToCoords(sf::Mouse::getPosition(mRenderWindow), mSimulationView);
}

void Application::packIntoLeftMenu(std::string const& tabTitle, sfg::Widget::Ptr widget)
{
    // Find existing tab or create new one
    auto tab = findLeftMenuTab(tabTitle);
    if (!tab) {
        auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10);
        auto scroll = sfg::ScrolledWindow::Create();
        scroll->SetRequisition({ 0, static_cast<float>(getSettingsSize().y - 50) });
        scroll->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC);
        scroll->SetPlacement(sfg::ScrolledWindow::Placement::TOP_LEFT);
        scroll->AddWithViewport(box);
        auto index = mLeftTabs->AppendPage(scroll, sfg::Label::Create(tabTitle));
        tab = mLeftTabs->GetNthPage(index);
    }

    // Make sure we have a scroll box in which we can pack the new widget
    auto scroll = std::dynamic_pointer_cast<sfg::ScrolledWindow>(tab);
    assert(scroll);
    auto viewport = scroll->GetViewport();
    assert(viewport);
    auto children = viewport->GetChildren();
    assert(children.size() == 1);
    auto box = std::dynamic_pointer_cast<sfg::Box>(children[0]);
    assert(box);

    box->Pack(widget);
}

void Application::packIntoLeftMenuNewSlider(std::string const& tabTitle, std::string const& name,
                                            std::list<std::string> const& property, double min, double max, double step)
{
    // Create a title, a slider and a label for the current value
    auto sliderTitle = sfg::Label::Create(name);
    auto slider = sfg::Scale::Create(min, max, step, sfg::Scale::Orientation::HORIZONTAL);
    slider->SetRequisition({ static_cast<float>(getSettingsSize().x - 75), 20 });
    auto sliderLabel = sfg::Label::Create("Value");
    auto adjustment = slider->GetAdjustment();

    // Setup callback
    auto callback = [property, adjustment, sliderLabel] {
        auto newValue = adjustment->GetValue();
        auto stringValue = std::to_string(newValue); // gives something like 0.330000
        stringValue = stringValue.substr(0, 4);      // This is obviously not perfect...
        sliderLabel->SetText(stringValue);
        j::getProperty(getAppConfig(), property) = j::number(newValue);
    };
    adjustment->GetSignal(sfg::Adjustment::OnChange).Connect(callback);

    // Set the initial value
    auto const& initialValue = j::getProperty(getConfig(), property).toDouble();
    adjustment->SetValue(initialValue);

    // Put everything inside a box
    auto textBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 10);
    textBox->Pack(sliderTitle, true, false);
    textBox->Pack(sliderLabel, false);
    auto sliderBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10);
    sliderBox->Pack(textBox);
    sliderBox->Pack(slider, true);

    // Populate left menu
    packIntoLeftMenu(tabTitle, sliderBox);
}

void Application::packIntoLeftMenuNewCheckbox(std::string const& tabTitle, std::string const& name,
                                              std::list<std::string> const& property)
{
    // Create the button with its initial value
    auto checkbox = sfg::CheckButton::Create(name);
    auto const& initialValue = j::getProperty(getConfig(), property).toBool();
    checkbox->SetActive(initialValue);

    // Setup callback
    auto callback = [property, checkbox] {
        auto newValue = checkbox->IsActive();
        j::getProperty(getAppConfig(), property) = j::boolean(newValue);
    };
    checkbox->GetSignal(sfg::ToggleButton::OnToggle).Connect(callback);

    // Populate left menu
    packIntoLeftMenu(tabTitle, checkbox);
}

void Application::addGraph(std::string const& title, std::vector<std::string> const& series, double min, double max)
{
    // Add a new tab for this graph
    auto index = mStatsTabs->AppendPage(mStatsFrame, sfg::Label::Create(title));

    auto const STATS_AREA = mStatsFrame->GetAllocation();
	// A DECOMMENTER APRES LE CODAGE DE LA CLASSE Stats
	 getStats().addGraph(index, title, series, min, max, { STATS_AREA.width, STATS_AREA.height });

    // Recreate the views
    createViews();
}

void Application::createWindow(Vec2d const& size)
{
    sf::VideoMode vm(size.x, size.y);

    auto title = getConfig()["window"]["title"].toString();

    sf::ContextSettings contextSettings;
    contextSettings.antialiasingLevel = getConfig()["window"]["antialiasing level"].toInt();

    // Create the window
    mRenderWindow.create(vm, title, sf::Style::Close, contextSettings);
    mRenderWindow.setKeyRepeatEnabled(true);
    mRenderWindow.setFramerateLimit(60);
}

sfg::Window::Ptr Application::createGUI()
{
    //   WINDOW STRUCTURE
    //
    //
    //     ⤹ mLeftBox
    //
    //   ------------------------
    //   | l |                  |
    //   | e |                  |
    //   | f | mSimulationFrame |
    //   | t |                  |
    //   |   |                  |  ⟵ upperBox
    //   | m |                  |
    //   | e |                  |
    //   | n |                  |
    //   | u |                  |
    //   ------------------------
    //   |    statistics        |  ⟵ mStatsTabs + mStatsFrame
    //   ------------------------

    // Simulation area
    mSimulationFrame = sfg::Frame::Create();
    mSimulationFrame->SetRequisition(getSimulationSize());

    // Left menu with its title
    mLeftBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10);

    // Left menu tabs
    mLeftTabs = sfg::Notebook::Create();
    mLeftTabs->SetScrollable(true);
    mLeftBox->Pack(mLeftTabs, true); // auto expand

    // Upper box: left menu + simulation area
    auto upperBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 10);
    upperBox->Pack(mLeftBox, true); // auto expand
    upperBox->Pack(sfg::Separator::Create(sfg::Separator::Orientation::VERTICAL), false);
    upperBox->Pack(mSimulationFrame, false);

    // Bottom panel: stats -> tabs to select the graph
    mStatsFrame = sfg::Frame::Create();
    mStatsFrame->SetRequisition(getStatsSize());
    mStatsTabs = sfg::Notebook::Create();
    mStatsTabs->SetScrollable(true);

    // Build the main layout
    auto mainBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5);
    mainBox->Pack(upperBox, false);
    mainBox->Pack(sfg::Separator::Create(sfg::Separator::Orientation::HORIZONTAL), false);
    mainBox->Pack(mStatsTabs, true); // auto expand

    // Create a desktop with a "fullscreen" window and add the main layout to it.
    auto window = sfg::Window::Create(sfg::Window::Style::BACKGROUND);
    window->SetRequisition(getWindowSize());
    window->Add(mainBox);

    return window;
}

void Application::createViews()
{
    mSimulationView = setupView(getWorldSize(), mSimulationFrame, mRenderWindow.getSize());
    mStatsView = setupView(getStatsSize(), mStatsFrame, mRenderWindow.getSize());
}

void Application::handleEvent(sf::Event event, sf::RenderWindow& window)
{
    switch (event.type) {
    case sf::Event::Closed:
        window.close();
        break;

    case sf::Event::KeyPressed:
        switch (event.key.code) {
        case sf::Keyboard::Escape:
            window.close();
            break;

        case sf::Keyboard::Z:
            zoomView(!event.key.shift); // if shift is pressed we zoom out
            break;

        case sf::Keyboard::R:
            getEnv().reset();
			// A DECOMMENTER APRES LE CODAGE DE LA CLASSE Stats
            getStats().reset();
            onSimulationStart();
            break;

        case sf::Keyboard::Left:
            moveView(MoveDirection::Left);
            break;

        case sf::Keyboard::Right:
            moveView(MoveDirection::Right);
            break;

        case sf::Keyboard::Up:
            moveView(MoveDirection::Up);
            break;

        case sf::Keyboard::Down:
            moveView(MoveDirection::Down);
            break;

        default:
            onEvent(event, window);
            break;
        } // event.key.code switch
        break;

    default:
        onEvent(event, window);
        break;
    } // event.type switch
}

void Application::render(sf::Drawable const& simulationBackground, sf::Drawable const& statsBackground)
{
    // Clear state and window content
    mRenderWindow.resetGLStates();
    mRenderWindow.clear(sf::Color::White);

    // Render the User Interface (SFGUI)
    mUIManager.Display(mRenderWindow);

    // Render the SFML part (simulation and stats)
    mRenderWindow.pushGLStates();
    {
        // Render the simulation
        mRenderWindow.setView(mSimulationView);
        mRenderWindow.draw(simulationBackground);
        getEnv().drawOn(mRenderWindow);

        // Render the stats
        mRenderWindow.setView(mStatsView);
        mRenderWindow.draw(statsBackground);
		// A DECOMMENTER APRES LE CODAGE DE LA CLASSEW Stats
        getStats().drawOn(mRenderWindow);
    }
    mRenderWindow.popGLStates();

    // Finally, publish everything onto the screen
    mRenderWindow.display();
}

void Application::zoomView(bool in)
{
    double const factor = in ? 1.0 / 1.5 : 1.5;

    mSimulationView.zoom(factor);

    mRenderWindow.setView(mSimulationView);
}

void Application::moveView(MoveDirection direction)
{
    Vec2d vect;
    switch (direction) {
    case MoveDirection::Left:
        vect = { -1, 0 };
        break;

    case MoveDirection::Right:
        vect = { 1, 0 };
        break;

    case MoveDirection::Up:
        vect = { 0, -1 };
        break;

    case MoveDirection::Down:
        vect = { 0, 1 };
        break;
    }

    float const factor = 100;
    mSimulationView.move(vect * factor);

    // Force the view within the limits of the world
    auto const world = getWorldSize();

    Vec2d center = mSimulationView.getCenter();
    if (center.x < 0)
        center.x = 0;
    if (center.x > world.x)
        center.x = world.x;
    if (center.y < 0)
        center.y = 0;
    if (center.y > world.y)
        center.y = world.y;
    mSimulationView.setCenter(center);

    mRenderWindow.setView(mSimulationView);
}

  // A DECOMMENTER APRES LE CODAGE DE LA CLASSE Stats
Stats& Application::getStats()
{
    return *mStats;
}

void Application::togglePause(sfg::CheckButton::Ptr button)
{
    mPaused = button->IsActive();
}

void Application::saveConfig() const
{
    //j::writeToFile(getConfig(), mAppDirectory + mCfgFile);
}

/*
void Application::importLab()
{
    // Try to load state from file but it we fail we revert to the current state
    auto currentState = getEnv().save();
    auto failed = false;

    try {
        auto filepath = mAppDirectory + RES_LOCATION + getConfig()["map"].toString();
        auto data = j::readFromFile(filepath);

        getEnv().reset();
        getStats().reset();

        getEnv().load(data);
    } catch (j::BadPayload const& e) {
        std::cerr << e.what() << std::endl;
        failed = true;
    } catch (j::NoSuchFile const& e) {
        std::cerr << e.what() << std::endl;
        failed = true;
    } catch (j::NoSuchElement const& e) {
        std::cerr << e.what() << std::endl;
        failed = true;
    } catch (j::BadConversion const& e) {
        std::cerr << e.what() << std::endl;
        failed = true;
    }

    if (failed) {
        getEnv().reset();
        getStats().reset();

        // Assuming the next call doesn't throw...
        getEnv().load(currentState);
    }
}

void Application::exportLab() const
{
    j::writeToFile(getEnv().save(), mAppDirectory + RES_LOCATION + getConfig()["map"].toString());
}
*/
sfg::Widget::Ptr Application::findLeftMenuTab(std::string const& tabTitle)
{
    auto maxIndex = mLeftTabs->GetPageCount();
    for (auto i = 0; i < maxIndex; ++i) {
        auto label = std::dynamic_pointer_cast<sfg::Label>(mLeftTabs->GetNthTabLabel(i));
        assert(label);
        auto title = label->GetText();
        if (title == tabTitle) {
            return mLeftTabs->GetNthPage(i);
        }
    }

    // Not found:
    return nullptr;
}

Application& getApp()
{
    assert(currentApp != nullptr);

    return *currentApp;
}

Lab& getAppEnv()
{
    return getApp().getEnv();
}

j::Value& getAppConfig()
{
    return getApp().getConfig();
}

sf::Font const& getAppFont()
{
    return getApp().getFont();
}

sf::Texture& getAppTexture(std::string const& name)
{
    return getApp().getTexture(name);
}

bool isDebugOn()
{
    return getAppConfig()["debug"].toBool();
}
