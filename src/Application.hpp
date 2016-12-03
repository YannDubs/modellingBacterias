/*
 * prjsv 2015
 * 2013, 2014
 * Marco Antognini
 */

#ifndef INFOSV_APPLICATION_HPP
#define INFOSV_APPLICATION_HPP

#include <Lab/Lab.hpp>
#include <JSON/JSON.hpp>
// A DECOMMENTER APRES LE CODAGE DE LA CLASSE Stats
#include <Stats/Stats.hpp>
#include <Utility/Vec2d.hpp>

#include <SFGUI/SFGUI.hpp>
#include <SFML/Graphics.hpp>

#include <list>
#include <map>
#include <string>

/*!
 * @class Application
 *
 * @brief Abstract class managing the core of the program
 *
 * Subclass can optionally re-implement onEvent(), onUpdate() and onDraw().
 *
 * The Lab class handles the drawing and update of the system.
 *
 * Note that `simulation` and `world` usually mean the same thing here.
 */
class Application
{
public:
    /*!
     * @brief Constructor
     *
     * @param argc argument count
     * @param argv launch arguments
     */
    Application(int argc, char const** argv);

    /// Forbid copy
    Application(Application const&) = delete;
    Application& operator=(Application const&) = delete;

    /*!
     * @brief Destructor
     */
    virtual ~Application();

    /*!
     * @brief Run the application
     *
     * This function is the main loop.
     *
     * @note Don't forget to call init() before run() !
     */
    void run();

    /*!
     * @brief Get access to the execution environment of the application (the lab)
     *
     * @note This breaks the encapsulation but simplify everything!
     *
     * @return the app's lab
     */
    Lab& getEnv();
    Lab const& getEnv() const;

    /*!
     * @brief Get access to the application's configuration
     *
     * @return the app's config
     */
    j::Value& getConfig();
    j::Value const& getConfig() const;

    /*!
     * @brief Get the app's font
     *
     * @return a font
     */
    sf::Font const& getFont() const;

    /*!
     * @brief Get a texture
     *
     * The texture is loaded on the fly from the 'res' folder and is owned
     * by this application.
     *
     * The returned texture is read-write capable but in most case a read-only
     * access should be enough.
     *
     * The application guarantees that the texture is not moved in memory and
     * is not destroyed until the destruction of the application. This ensures
     * that the returned reference will be valid until the end of the application
     * and therefore can be given to a sprite.
     *
     * If the corresponding texture couldn't be loaded a default white texture is
     * returned and an error message is displayed on sf::Err by SFML.
     *
     * @param name name of the texture
     * @return a reference to the corresponding texture
     */
    sf::Texture& getTexture(std::string const& name);

    /*!
     * @brief Get the path to the resource folder
     */
    std::string getResPath() const;

    /**
     *  @brief Read the world size from the config manager
     *
     *  @return the world size
     */
    Vec2d getWorldSize() const;

    /**
     *  @brief Compute the centre of the world area (in local coordinates)
     *
     *  @return the centre of the app
     */
    Vec2d getCentre() const;

protected:
    /*!
     *  @brief Called once before starting the main loop
     *
     *  Subclasses can override this method to, for example, add elements to the left menu.
     *  The default implementation adds a pause checkbox in the left menu.
     */
    virtual void onRun();

    /**
     *  @brief Called when the simulation is (re)started, but not when importing data
     *
     *  By default nothing is done but subclasses can, e.g., generate bacteria or swarm.
     */
    virtual void onSimulationStart();

    /*!
     * @brief Subclass can override this method to handle events
     *
     * The default implementation does nothing.
     *
     * @param event an event
     * @param window the window that emitted the event
     */
    virtual void onEvent(sf::Event event, sf::RenderWindow& window);

    /*!
     * @brief Subclass can override this method to update their data
     *
     * The default implementation does nothing. However, the env is always updated first.
     *
     * @param dt elapsed time
     */
    virtual void onUpdate(sf::Time dt);

    /*!
     * @brief Subclass can override this method to draw their data
     *
     * The default implementation does nothing. However, the env is always displayed first.
     *
     * @param target a render target
     */
    virtual void onDraw(sf::RenderTarget& target);

    /*!
     * @brief Get the cursor position in the view coordinates (i.e. pixel coordinates)
     *
     * @return The cursor position converted in the view coordinates
     */
    Vec2d getCursorPositionInView() const;

    /*!
     *  @brief Add a widget to the left menu
     *
     *  @param tabTitle title of the tab in which the widget should go, new tabs are automatically created
     *  @param widget a new widget
     */
    void packIntoLeftMenu(std::string const& tabTitle, sfg::Widget::Ptr widget);

    /**
     *  @brief Add a slider that is bound to some config settings
     *
     *  @param tabTitle title of the tab in which the widget should go, new tabs are automatically created
     *  @param name     name to be displayed
     *  @param property property path
     *  @param min      min value
     *  @param max      max value
     *  @param step     precision of the slider
     */
    void packIntoLeftMenuNewSlider(std::string const& tabTitle, std::string const& name,
                                   std::list<std::string> const& property, double min, double max, double step);

    /**
     *  @brief Add a checkbox that is bound to some config settings
     *
     *  @param tabTitle title of the tab in which the widget should go, new tabs are automatically created
     *  @param name     name to be display
     *  @param property property path
     */
    void packIntoLeftMenuNewCheckbox(std::string const& tabTitle, std::string const& name,
                                     std::list<std::string> const& property);

    /**
     *  @brief Add a graph to the stats manager and update GUI
     *
     *  @param title  graph's title
     *  @param series series' title
     *  @param min    y-axis: min value expected
     *  @param max    y-axis: max value expected
     */
    void addGraph(std::string const& title, std::vector<std::string> const& series, double min, double max);

private:
    /*!
     * @brief Create the window
     */
    void createWindow(Vec2d const& size);

    /*!addG
     *  @brief Create the graphical user interface
     *
     *  This method also sets up mSimulationFrame, mLeftBox
     *
     *  @return a "fullscreen" GUI window
     */
    sfg::Window::Ptr createGUI();

    /*!
     *  @brief Create the simulation and stats views
     *
     *  @note call createGUI() first
     */
    void createViews();

    /*!
     * @brief Do the logic for a given event
     *
     * @param event Event to handle
     * @param window the window that emitted the event
     */
    void handleEvent(sf::Event event, sf::RenderWindow& window);

    /*!
     *  @brief Render the GUI, Simulation and Stats
     *
     *  @param simulationBackground Background of the simulation frame
     *  @param statsBackground      Background of the stats frame
     */
    void render(sf::Drawable const& simulationBackground, sf::Drawable const& statsBackground);

    void zoomView(bool in);

    enum class MoveDirection { Left, Right, Up, Down };
    void moveView(MoveDirection direction);

    /**
     *  @brief Get access to the stats manager
     *
     *  @return the application statistic manager
     */
	// A DECOMMENTER APRES LE CODAGE DE LA CLASSE Stats
      Stats& getStats();

private:
    // Left menu events
    void togglePause(sfg::CheckButton::Ptr button);
    void saveConfig() const;
//    void importLab();
//    void exportLab() const;

    // Get a specific tab, or a null pointer if it doesn't exist
    sfg::Widget::Ptr findLeftMenuTab(std::string const& tabTitle);

private:
    // The order is important since some fields need other to be initialised
    std::string const mAppDirectory; ///< Path to the executable's directory
    std::string const mCfgFile;      ///< Relative path to the CFG
    j::Value mConfig;                ///< Application configuration
    Lab* mEnv;                       ///< Simulated lab
    sf::Font mFont;                  ///< A font

    sf::RenderWindow mRenderWindow;   ///< SFML window / render target
    sfg::SFGUI mUIManager;            ///< SFGUI manager
    sfg::Frame::Ptr mSimulationFrame; ///< Simulation Frame
    sfg::Box::Ptr mLeftBox;           ///< Left menu
    sfg::Notebook::Ptr mLeftTabs;     ///< Left menu tabs
    sf::View mSimulationView;         ///< View for simulation area

	// A DECOMMENTER APRES LE CODAGE DE LA CLASSE Stats
      Stats* mStats;                 ///< Statistic manager
    sf::View mStatsView;           ///< View for the stats area
    sfg::Frame::Ptr mStatsFrame;   ///< Stats frame where the graph are drawn
    sfg::Notebook::Ptr mStatsTabs; ///< Tab for graph selection

    using TexturePool = std::map<std::string, sf::Texture*>;
    TexturePool mTextures;       ///< Pool of textures
    sf::Texture mDefaultTexture; ///< Default, white texture
    // mDefaultTexture is used when a texture in the pool is not available

    bool mPaused; ///< Tells if the application is in pause or not
};

/*!
 * @brief Get the current instance of Application
 *
 * @return a reference to the current instance of Applicaton
 */
Application& getApp();

/*!
 * @brief Get the environment (the lab) of the current application
 *
 * Shorthand for getApp().getEnv()
 *
 * @see Application::getEnv() comment about encapsulation
 *
 * @return the app's env.
 */
Lab& getAppEnv();

/*!
 * @brief Get the config of the current application
 *
 * Shorthand for getApp().getConfig()
 *
 * @return the app's config
 */
j::Value& getAppConfig();

/*!
 * @brief Get the app's font
 *
 * Shorthand for getApp().getFont()
 *
 * @return a font
 */
sf::Font const& getAppFont();

/*!
 * @brief Get a texture
 *
 * Shorthand for getApp().getTexture(name)
 *
 * @param name name of the texture
 * @return a reference to the texture
 *
 * @see Application::getTexture
 */
sf::Texture& getAppTexture(std::string const& name);

/*!
 * @brief Determine if debug mode is active or not
 *
 * Shorthand for getAppConfig().getBool(DEBUG_MODE)
 *
 * @return true if cfg specify DEBUG=TRUE
 */
bool isDebugOn();


/// Define a few macros


/*!
 * @brief Implement the main() function with a given subclass of Application
 *
 * @param APPLICATION_CLASS a class name (it must be a subclass of Application)
 */
#define IMPLEMENT_MAIN(APPLICATION_CLASS)  \
    int main(int argc, char const** argv)  \
    {                                      \
        APPLICATION_CLASS app(argc, argv); \
        app.run();                         \
        return 0;                          \
    }

#define INIT_DEFAULT_APP(argc, argv) Application app((argc), (argv));


#endif // INFOSV_APPLICATION_HPP
