#include "Gui.h"


Gui::Gui()
{
	this->display = NULL;
	this->flipTimer = NULL;
	this->queue = NULL;
	this->ev = {NULL};

    this->err = lcdError();

    this->displaySizeX = WIDTH;
    this->displaySizeY = HEIGHT;
    this->fps = FPS;

    this->running = true;
    this->popupOpen = false;

    this->test = 0;
}   



bool Gui::showMainWindow(void) 
{
    return true;
}

void Gui::mainWindow(void)
{
    static bool NoTitlebar = false;
    static bool NoMenu = true;
    static bool NoCollapse = false;
    static bool NoResize = false;
    static bool NoMove = false;
    static bool NoClose = true;
    static bool NoBackground = false;
    static bool NoScrollbar = true;
    static bool no_bring_to_front = false;

    ImGuiWindowFlags window_flags = 0;
    if (NoTitlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (NoScrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (!NoMenu)           window_flags |= ImGuiWindowFlags_MenuBar;
    if (NoMove)            window_flags |= ImGuiWindowFlags_NoMove;
    if (NoResize)          window_flags |= ImGuiWindowFlags_NoResize;
    if (NoCollapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
    if (NoBackground)      window_flags |= ImGuiWindowFlags_NoBackground;
    if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::SetNextWindowSize(ImVec2(displaySizeX, displaySizeY), ImGuiCond_Always); //Aca pongo tamaño de la pantalla
    ImGui::Begin("Configure Simulation2", NULL, window_flags);

    ImGui::RadioButton("Display a", &test, 1); ImGui::SameLine();
  

    ImGui::RadioButton("Display b", &test, 2); ImGui::SameLine();

    ImGui::RadioButton("Display c", &test, 3);

    if (ImGui::Button("Show Test"))
    {
        std::cout << test << std::endl;
    }
   
    ImGui::End();
 
}


void Gui::dispatcher(void)
{
    ImGui_ImplAllegro5_ProcessEvent(&ev);
    switch (ev.type)
    {
    case ALLEGRO_EVENT_TIMER:
        this->refresh();
        break;
    case ALLEGRO_EVENT_DISPLAY_CLOSE:
        running = false;
        break;
    case ALLEGRO_EVENT_DISPLAY_RESIZE:
        this->resize();
        break;
    default:
        break;
    }
}

bool Gui::getEvent(void)
{
    return al_get_next_event(queue, &ev);
}


void Gui::refresh(void)
{
    ImGui_ImplAllegro5_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);

    mainWindow();

    ImGui::Render();

    al_clear_to_color(al_map_rgba_f(1, 1, 0.8, 1));

    //Todo lo que dibuje aca va a quedar por detrás de las ventanas de DearImGui

    ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());

    //Todo lo que dibuje aca va a quedar por encima de las ventanas de DearImGui

    al_flip_display();
}

void Gui::initAll(void)
{
    try
    {
        initAllegro();
        initDisplay();
        initTimer();
        initEvents();
    }
    catch (errorClass errCode)
    {
        this->err.setErrorCode(errCode);
        std::cout << this->err.getErrorDescription() << std::endl;
    }

    al_start_timer(flipTimer);
    initImGui();

}

void Gui::initAllegro(void)
{
    if (!al_init()) 
    {
        throw(errorClass::AL_INIT_ERR);
    }

    if (!al_init_primitives_addon()) 
    {
        throw(errorClass::AL_INIT_PRIMITIVES_ERR);
    }

    if (!al_install_keyboard()) 
    {
        throw(errorClass::AL_INIT_KEYBOARD_ERR);
    }

    if (!al_install_mouse()) 
    {
        throw(errorClass::AL_INIT_MOUSE_ERR);
    }

    if (!al_init_image_addon()) 
    {
        throw(errorClass::AL_INIT_IMAGE_ERR);
    }
}

void Gui::initDisplay(void)
{
    using namespace std;
    
    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    display = al_create_display(displaySizeX, displaySizeY);
    if (!display) 
    {
        throw(errorClass::AL_CREATE_DISPLAY_ERR);
    }
    
    al_set_target_backbuffer(this->display);
    al_set_window_title(display, "Twitter Downloading Interface");
    
}

void Gui::initTimer(void)
{
    flipTimer = al_create_timer(1 / fps);
    if (!flipTimer)
    {
        throw(errorClass::AL_CREATE_TIMER_ERR);
    }
}

void Gui::initEvents(void)
{
    queue = al_create_event_queue();

    if (!queue)
    {
        throw(errorClass::AL_CREATE_EVQUEUE_ERR);
    }

 
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());

    al_register_event_source(queue, al_get_timer_event_source(flipTimer));
    
}

void Gui::initImGui(void)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplAllegro5_Init(display);
    ImGui::StyleColorsLight();
}

void Gui::destroyAll(void)
{
    ImGui_ImplAllegro5_Shutdown();
    ImGui::DestroyContext();

    al_destroy_event_queue(queue);
    al_destroy_display(display);
    al_shutdown_primitives_addon();
}

void Gui::resize(void)
{
    ImGui_ImplAllegro5_InvalidateDeviceObjects();
    al_acknowledge_resize(display);
    ImGui_ImplAllegro5_CreateDeviceObjects();
}

bool Gui::isOver(void)
{
    return !running;
}

bool Gui::noError(void)
{
    if (err.getErrorCode() == errorClass::NO_ERR)
    {
        return true;
    }
    else
    {
        return false;
    }
}