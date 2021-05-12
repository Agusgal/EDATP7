#include "Gui.h"

namespace 
{
    const int loadingDotsNumber = 3;
    const double timeDots = 0.1;
    const double timeRollInit = 0.15;

    const double minTimeRoll = 0.08;
    const double maxTimeRoll = 0.3;
}

Gui::Gui()
{
	this->display = NULL;
	this->flipTimer = NULL;
	this->queue = NULL;
	this->ev = {NULL};

    this->rollVar = timeRollInit;

    this->requestTimer = NULL;
    this->requestEv = { NULL };

    this->err = lcdError();

    this->rollTimer = NULL;

    this->displaySizeX = WIDTH;
    this->displaySizeY = HEIGHT;
    this->fps = FPS;

    this->running = true;
    this->popupOpen = false;

    this->test = 0;

    this->tweetNumber = 0;

    this->comboItems[0] = "Display A";
    this->comboItems[1] = "Display B";
    this->comboItems[2] = "Display C";

    this->currentItemId = 0;
    this->rollTwitts = false;
    this->positionRoll = 0;

    this->allegroEventComes = false;
    this->genericEventComes = false;

    this->displayInUSe = NULL;

    //Better in initgui
    this->tClient = new (std::nothrow) TwitterHandler;
    this->tClient->init();
    if (!tClient)
        throw std::exception("Failed to allocate memory for Twitter client.");

    this->loadState = state::TCnotLoaded;
}   


bool Gui::showMainWindow(void) 
{
    return true;
}

void Gui::testWindow(void)
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

void Gui::popup(const char* msg)
{
    ImGui::OpenPopup("error");
    if (ImGui::BeginPopup("error")) 
    {
        ImGui::Text(msg);
        ImGui::EndPopup();
    }
}

void Gui::mainWindow(void)
{
    ImGui::SetNextWindowSize(ImVec2(displaySizeX, displaySizeY), ImGuiCond_Always); //Aca pongo tamaño de la pantalla
    ImGui::Begin("Configure Display", NULL);

    // Child 1 ----> Display generation 
    {
        ImGui::BeginChild("ChildL", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, displaySizeY), false);
        
        const char* combo_label = comboItems[currentItemId];
        if (ImGui::BeginCombo("Display Selector", combo_label))
        {
            for (int n = 0; n < IM_ARRAYSIZE(comboItems); n++)
            { 
                const bool is_selected = (currentItemId == n);
                if (ImGui::Selectable(comboItems[n], is_selected))
                {
                    currentItemId = n;
                    std::cout << currentItemId << std::endl;
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        
        bool available = idAvailable(comboItems[currentItemId]);
        bool createDisp = ImGui::Button("Create Display");
        if (createDisp && available)
        {            
            userInput.push_back(new UserData());
            userInput.back()->setOpen();
            userInput.back()->setId(comboItems[currentItemId]);

            switch (currentItemId)
            {
            case 0:
                displays.push_back(new LcdA());
                break;
            case 1:
                displays.push_back(new LcdB());
                break;
            case 2:
                displays.push_back(new LCD3());
                break;
            default:
                break;
            }
        }
        else if (createDisp && !available)
        {
            std::string error = "Display Already created";
            popup(error.c_str());
        }
        ImGui::EndChild();
    }

    ImGui::SameLine();

    //Child 2 ----> Display control Tabs
    {
        ImGui::BeginChild("ChildR", ImVec2(0, displaySizeY), false);
        
        if (ImGui::BeginTabBar("MyTabBar"))
        {
            for (const auto& input : userInput)
                if (input->isOpen() && ImGui::BeginTabItem(input->getIdNR().c_str(), &(input->getOpenFlag())))
                {
                    
                    ImGui::Text("This is the %s tab!", input->getIdNR().c_str());
                    
                    ImGui::myInputText("Enter Twitter User", &(input->getUser()));
                    ImGui::InputInt("Enter Twitt N", &(input->getTwittN()));

                    if (ImGui::Button("Download Twitts"))
                    {
                        genericEventComes = true;
                        this->displayInUSe = getCurrentDisplay(input->getIdNR());
                        nextEvent = events::REQUEST;
                    }

                    ImGui::SameLine();
                    if (ImGui::Button("Cancel Download"))
                    {
                        genericEventComes = true;
                        this->displayInUSe = getCurrentDisplay(input->getIdNR());
                        this->nextEvent = events::CANCELREQUEST;
                    }

                    if (ImGui::Button("Next Tweet"))
                    {
                        genericEventComes = true;
                        this->displayInUSe = getCurrentDisplay(input->getIdNR());
                        this->nextEvent = events::NEXT;
                    }

                    ImGui::SameLine();
                    if (ImGui::Button("Previous Tweet"))
                    {
                        genericEventComes = true;
                        this->displayInUSe = getCurrentDisplay(input->getIdNR());
                        this->nextEvent = events::PREVIOUS;
                    }

                    
                    ImGui::SliderFloat("Roll Speed", &rollVar, minTimeRoll, maxTimeRoll, "%.3f");
                    al_set_timer_speed(rollTimer, (double)rollVar);


                    if (ImGui::Button("Close Display"))
                    {
                        genericEventComes = true;
                        this->displayInUSe = getCurrentDisplay(input->getIdNR());
                        this->nextEvent = events::CLOSE;
                    }

                    ImGui::EndTabItem();
                }
            ImGui::EndTabBar();
        }
        
        
        ImGui::EndChild();
    }


    ImGui::End();
}


void Gui::dispatcher(void)
{
    if (allegroEventComes)
    {
        ImGui_ImplAllegro5_ProcessEvent(&ev);
        switch (ev.type)
        {
        case ALLEGRO_EVENT_TIMER:
            if (ev.timer.source == flipTimer)
            {
                this->refresh();
                break;
            }
            else if ((ev.timer.source == rollTimer) && rollTwitts)
            {
                this->roll();
                break;
            }
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
        allegroEventComes = false;
    }
    else if (genericEventComes)
    {
        switch (nextEvent)
        {
        case events::REQUEST:
            tweetNumber = 0;
            parseToClient(userInput[displayInUSe]->getUserNR().c_str(), userInput[displayInUSe]->getTwittNR(), userInput[displayInUSe]->getIdNR());
            performRequest(userInput[displayInUSe]->getIdNR());
            break;
        case events::CANCELREQUEST:
            if (!displays[displayInUSe]->lcdClear())
                throw std::exception("Failed to clear LCD.");
            rollTwitts = false;
            loadState = state::TCnotLoaded;
            break;
        case events::ROLL:
            roll();
            break;
        case events::NEXT:
            if (loadState == state::TweetsRequested)
            {
                showNextTweet();
            }
            break;
        case events::PREVIOUS:
            if (loadState == state::TweetsRequested)
            {
                showPreviousTweet();
            }
            break;
        case events::CLOSE:
            delete displays[displayInUSe];
            delete userInput[displayInUSe];

            displays.erase(displays.begin() + displayInUSe);
            userInput.erase(userInput.begin() + displayInUSe);
            rollTwitts = false;
            loadState = state::TCnotLoaded;
            break;
        default:
            break;
        }
        genericEventComes = false;
    }
}

bool Gui::getEvent(void)
{
    if (al_get_next_event(queue, &ev))
    {
        allegroEventComes = true;
        return true;
    }
    else if (genericEventComes)
    {
        return true;
    }
    else
    {
        return false;
    }
}


void Gui::refresh(void)
{
    refreshLCDs();
    refreshImgui();
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
    al_start_timer(rollTimer);
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

    if (!al_init_font_addon())
    {
        throw(errorClass::AL_INIT_FONT_ERR);
    }

    if (!al_init_ttf_addon())
    {
        throw(errorClass::AL_INIT_TTF_ERR);
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

    requestTimer = al_create_timer(timeDots);
    if (!requestTimer)
    {
        throw(errorClass::AL_CREATE_TIMER_ERR);
    }

    rollTimer = al_create_timer(timeRollInit);
    if (!rollTimer)
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
    al_register_event_source(queue, al_get_timer_event_source(requestTimer));

    al_register_event_source(queue, al_get_timer_event_source(rollTimer));
    
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


    for (auto el : displays)
    {
        delete el;
    }

    for (auto el : userInput)
    {
        delete el;
    }

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


void Gui::refreshImgui(void)
{
    al_set_target_bitmap(al_get_backbuffer(display));

    ImGui_ImplAllegro5_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(displaySizeX, displaySizeY), ImGuiCond_Always);


#ifdef DEMO

    ImGui::ShowDemoWindow();

#else 

    mainWindow();

#endif

    ImGui::Render();
    al_set_target_bitmap(al_get_backbuffer(display));//if i dont write this line the lcd display die xd
    al_clear_to_color(al_map_rgba_f(1, 1, 0.8, 1));

    //Todo lo que dibuje aca va a quedar por detrás de las ventanas de DearImGui

    ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());

    //Todo lo que dibuje aca va a quedar por encima de las ventanas de DearImGui

    al_flip_display();
}


void Gui::refreshLCDs(void)
{
    //if a tab was closed, then we update the program deleting the display and the info. 
    for (std::size_t n = 0; n < displays.size(); n++)
    {
        if (!userInput[n]->isOpen() && displays[n])
        {
            delete displays[n];
            delete userInput[n];

            displays.erase(displays.begin() + n);
            userInput.erase(userInput.begin() + n);
        }
    }
}


bool Gui::idAvailable(std::string id)
{
    for (auto& input : userInput)
    {
        if (input->getIdNR() == id)
        {
            return false;
        }
    }
    return true;
}

int Gui::getCurrentDisplay(std::string displayId)
{
    for (int n = 0; n < userInput.size(); n++)
    {
        if (userInput[n]->getIdNR() == displayId)
        {
            return n; 
        }
    }
}



void Gui::parseToClient(const char* username, int tweetN, std::string displayId)
{
    
    int id = getCurrentDisplay(displayId);

    try {
        tClient->newUsername(username);

        if (tweetN)
        {
            tClient->newtweetCount(tweetN);
        }

       
        if (!displays[id]->lcdClear())
        {
            throw std::exception("Failed to clear LCD.");
        }
        *displays[id] << username;
       

        loadState = state::TCLoaded;
    }
    catch (API_request_error& e) 
    {
       
        if (!displays[id]->lcdClear())
        {
            throw std::exception("Failed to clear LCD.");
        }
        *displays[id] << (char*)e.what();
    }
}


void Gui::performRequest(std::string displayId)
{
    int id = getCurrentDisplay(displayId);
    

    //Sets variables to use in function.
    bool going = true;

    int sign = 0;
    al_start_timer(requestTimer);
    //Attempts to set timer resources.
    try { 
        while (going)
        {
            if (al_get_next_event(queue, &ev))
            {
                allegroEventComes = true;
            }
            
            
            //Checks if there's been a timer event.
            if (al_get_next_event(queue, &requestEv))
            {
                loading(sign, id);
            }
            
            if (allegroEventComes)
            {
                ImGui_ImplAllegro5_ProcessEvent(&ev);
                if ((ev.type == ALLEGRO_EVENT_TIMER) && (ev.timer.source == flipTimer))
                {
                    refreshImgui();
                    allegroEventComes = false;
                }
            }


            going = tClient->requestTweet();

            if (this->nextEvent == events::CANCELREQUEST)
            {
                going = false;
            }
            else if (this->nextEvent == events::CLOSE)
            {
                going = false;
            }
        }
        //Destroys timer resources.
        al_stop_timer(this->requestTimer);

        //Clears LCD and writes first tweet's date and content.
        if (!displays[getCurrentDisplay(displayId)]->lcdClear())
        {
            throw std::exception("Failed to clear LCD");
        }
        if (tClient->getTweets().size()) 
        {
            std::cout << tClient->getTweets()[0].getUser() << std::endl;
                
            *displays[id] << (char*)tClient->getTweets()[0].getDate().c_str();
            displays[id]->lcdMoveCursorDown();
            *displays[id] << (char*)tClient->getTweets()[0].getContent().c_str();
            loadState = state::TweetsRequested;
                
            //Roll the twitts
            rollTwitts = true;
            positionRoll = 0;
        }
        else 
        {
            *displays[id] << (char*)"No tweets.";
            loadState = state::TCnotLoaded;
        }
        
    }
    catch (API_request_error& e)
    {
        if (!displays[id]->lcdClear())
            throw std::exception("Failed to clear LCD");
        *displays[id] << (char*)e.what();
    }
}

void Gui::loading(int& sign, int id)
{
    cursorPosition temp;
    temp.row = 1;
    temp.column = 0;

    if (!sign)
    {
        if (!displays[id]->lcdSetCursorPosition(temp))
        {
            throw std::exception("Failed to set cursor's position.");
        }
        if (!displays[id]->lcdClearToEOL())
        {
            throw std::exception("Failed to clear to EOL in LCD.");
        }

        *displays[id] << (char*)"Requesting";
    }
    else
    {
        *displays[id] << (char)'.';

        if (sign == loadingDotsNumber)
        {
            sign = -1;
        }
    }
    sign++;
}

void Gui::roll(void) 
{
    unsigned int totTweets = tClient->getTweets().size();
    if (totTweets && tweetNumber < totTweets) 
    {
        std::string tweet = tClient->getTweets()[tweetNumber].getContent();

        if (positionRoll < tweet.length())
        {
            cursorPosition temp; 
            temp.row = 1; 
            temp.column = 0;
            
            if (!displays[displayInUSe]->lcdSetCursorPosition(temp))
                throw std::exception("Failed to set cursor position.");
            
            std::string strToShow = tweet.substr(positionRoll, tweet.size());
            *displays[displayInUSe] << (char*)strToShow.c_str();
            positionRoll++;
        }
        else
        {
            showNextTweet();
        }
    }
}

//Shows next tweet or shows error message in lcd if there are no more tweets.
void Gui::showNextTweet() 
{
    try 
    {
        //Notice of last tweet.
        if (tweetNumber >= (tClient->getTweets().size() - 1)) 
        {
            tweetNumber = tClient->getTweets().size();
            if (!displays[displayInUSe]->lcdClear())
                throw std::exception("Failed to clear LCD");
            *displays[displayInUSe] << (char*)"No more tweets.";
        }
        else //Shows next tweet and updates tweetNumber.
        {
            tweetNumber++;
            if (!displays[displayInUSe]->lcdClear())
                throw std::exception("Failed to clear LCD");
            displays[displayInUSe]->lcdMoveCursorUp();
            *displays[displayInUSe] << (char*)tClient->getTweets()[tweetNumber].getDate().c_str();

            displays[displayInUSe]->lcdMoveCursorDown();
            *displays[displayInUSe] << (char*)tClient->getTweets()[tweetNumber].getContent().c_str();
            rollTwitts = true;
            positionRoll = 0;
        }
    }
    catch (std::exception&) 
    {
        return;
    }
}


//Shows previous tweet (if there is one).
void Gui::showPreviousTweet()
{
    try 
    {
        //Does nothing if there isn't a previous tweet.
        if (tweetNumber <= 0) 
        {
            return;
        }

        //Shows previous tweet and updates tweetNumber.
        else {
            tweetNumber--;
            if (!displays[displayInUSe]->lcdClear())
                throw std::exception("Failed to clear LCD.");
            displays[displayInUSe]->lcdMoveCursorUp();
            *displays[displayInUSe] << (char*)tClient->getTweets()[tweetNumber].getDate().c_str();

            displays[displayInUSe]->lcdMoveCursorDown();
            *displays[displayInUSe] << (char*)tClient->getTweets()[tweetNumber].getContent().c_str();
            rollTwitts = true;
            positionRoll = 0;
        }
    }
    catch (std::exception&) {
        return;
    }
}