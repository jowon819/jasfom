/* This Created By Jowon. */
/* This is named jasfom means just a smart friend of me. */
/* This is Jowon's Personal secretary. */
/* Jowon Think this as a friend. */
/* Jasfom was born 2015. 4. 17 */
/* This is the first Version. 1.0 */

#include <iostream>
#include <vector>
#include <cstdio>
#include <time.h>
#include <unistd.h>

#define STRINGSIZE 200

using namespace std;

void outputJasfomInfo();
bool pipeOpen(const char * comm);
char * pipeOpen(const char * comm, const char * type);
void informWeather();
void informTime();
void readyPintos();
void outputMacInfo();
void man();
void exitMac();

int main(int argc, const char*argv[])
{
    string args;
    string parameters[8] = {"who", "pintos", "weather", "wt", "time", "man", "amac", "exit"};
    
    if (argc > 1){

        /* when "jasfom who" command */
        if (!(strcmp(argv[1], parameters[0].c_str())))
        {
            outputJasfomInfo();
        }
        
        /* when "jasfom pintos" command */
        if(!(strcmp(argv[1], parameters[1].c_str())))
        {
            readyPintos();
        }
        
        /* when "jasfom wt or weather" command */
        if (!(strcmp(argv[1], parameters[2].c_str())) || !(strcmp(argv[1], parameters[3].c_str()))){
            informWeather();
        }
        
        /* when "jasfom time" command */
        if (!(strcmp(argv[1], parameters[4].c_str()))){
            informTime();
        }
        /* when "jasfom man" command */
        if (!(strcmp(argv[1], parameters[5].c_str()))){
            man();
        }

        /* when "jasfom amac" command */
        if (!(strcmp(argv[1], parameters[6].c_str()))){
            outputMacInfo();
        }
        
        /* when "jasfom exit" command */
        if (!(strcmp(argv[1], parameters[7].c_str()))){
            exitMac();
        }
    }
    else {
        /* when just input jasfom*/
        cout << "Jasfom speaking..." << endl;
        cout << "   What can I help you?, Sir." << endl;
        pipeOpen("say What can I help you?, Sir.");
    }
    return 0;
}

void outputJasfomInfo(){
    cout << "Jasfom speaking..." << endl;
    cout << "\nI am your secretary " << endl;
    cout << "     ________  __          _________  ________  ________    __    __        " << endl;
    cout << "    /____  _/ /  \\        / _______/ / ______/ / ____   \\  /  \\  /  \\       " << endl;
    cout << "        / /  / /\\ \\       \\ \\___    / /_____  / /    \\  / / /\\ \\/ /\\ \\      " << endl;
    cout << "   __  / /  / /__\\ \\       \\___ \\  / ______/ / /     / / / /  \\__/  \\ \\     " << endl;
    cout << "   \\ \\/ /  /  ____  \\   ______/ / / /       /  \\____/ / / /          \\ \\    " << endl;
    cout << "    \\__/  /_/      \\_\\ /_______/ /_/        \\________/ /_/            \\_\\   " << endl << endl;
    pipeOpen("say I am your secretary Jasfom, Sir.");
}

/* To execute command */
bool pipeOpen(const char * comm){
    FILE * file = popen(comm, "r");
    if (file == NULL)
        return false;
    pclose(file);
    return true;
}

/* To get char string from file */
/* return type : char * */
/* parameters : char * comm, char * type */
char * pipeOpen(const char * comm, const char * type) {
    static char buffer[STRINGSIZE];
    FILE * file = popen(comm, "r");
    fgets (buffer, STRINGSIZE, file);
    pclose (file);
    
    return buffer;
}

/* To display weather information : this function use function - pipOpen() */
void informWeather() {
    
    // today weather information
    vector<string> todayWInfo;
    // tonight weather information
    vector<string> tonightWInfo;
    
    // string of param
    char popenParam[STRINGSIZE];
    
    // to input vector<string token>
    string tempForToken;
    
    // token saver
    char * token;
    
    // weather information saver
    char * weatherInfo;
    
    /* TODAY procedure */
    /* pipe open to get TODAY's weather infomation, and save to buffer */
    
    weatherInfo = pipeOpen("curl -s https://weather.yahoo.com/south-korea/seoul/seoul-1132599/ | awk '/Today -/' | textutil -convert txt -stdin -stdout -format html", "r");
    
    /* tokenizing procedure */
    token = strtok (weatherInfo, ".");
    
    /* tokenize and push it to today's vector */
    while (token != NULL)
    {
        tempForToken = token;
        todayWInfo.push_back (tempForToken);
        tempForToken.clear();
        token = strtok (NULL, ".");
    }
    
    /* TONIGHT procedure */
    /* pipe open to get TONIGHT's weather infomation, and save to buffer */
    weatherInfo = pipeOpen("curl -s https://weather.yahoo.com/south-korea/seoul/seoul-1132599/ | awk '/Tonight -/' | textutil -convert txt -stdin -stdout -format html", "r");
    
    /* tokening procedure */
    token = strtok (weatherInfo, ".");

    /* tokenize and push it to tonight's vector */
    while (token != NULL)
    {
        tempForToken = token;
        tonightWInfo.push_back (tempForToken);
        tempForToken.clear();
        token = strtok (NULL, ".");
    }
    
    /* to erase last null string in vector */
    tonightWInfo.pop_back();
    todayWInfo.pop_back();
    
    /* check TODAY's chance of rain. */
    
    int flag = 0;
    /* when TODAY vector's last string's first token is "chance of ..." */
    if ( !strncmp(todayWInfo.back().c_str(), " Chance", 7) ){
        flag = 1;
    }
    
    /* check TONIGHT's chance of rain. */
    if ( !strncmp(tonightWInfo.back().c_str(), " Chance", 7) && flag == 1){
        flag = 3;
    }else if ( !strncmp(tonightWInfo.back().c_str(), "Chance", 7) && flag == 0 ){
        flag = 2;
    }
    
    cout << "Jasfom speaking..." << endl;
    cout << "    Seoul, South Korea" << endl;
    
    switch (flag) {
            
            /* There is no information about chance of rain. */
        case 0:
            snprintf(popenParam, STRINGSIZE, "say %s, and  %s,  Sir",
                     todayWInfo[0].c_str(), tonightWInfo[0].c_str());
            /* output info */
            cout << "    " << todayWInfo[0] << "." << endl;
            cout << "    " << tonightWInfo[0] << "." << " Sir."<< endl;
            break;
            
            /* Just today has information about chanc of rain. */
        case 1:
            snprintf(popenParam, STRINGSIZE, "say %s, and  %s, %s,  Sir",
                     todayWInfo[0].c_str(), todayWInfo.back().c_str(), tonightWInfo[0].c_str());
            /* output info */
            cout << "    " << todayWInfo[0] << "." << todayWInfo.back().c_str() << "." << endl;
            cout << "    " << tonightWInfo[0] << "." << " Sir."<< endl;
            
            break;
            
            /* Just tonight has information about chanc of rain. */
        case 2:
            snprintf(popenParam, STRINGSIZE, "say %s, and  %s, %s, Sir",
                     todayWInfo[0].c_str(), tonightWInfo[0].c_str(), tonightWInfo.back().c_str());
            /* output info */
            cout << "    " << todayWInfo[0] << "." << endl;
            cout << "    " << tonightWInfo[0] << "." << tonightWInfo.back() <<" Sir."<< endl;
            break;
            
            /* Both has information about chance of rain. */
        case 3:
            snprintf(popenParam, STRINGSIZE, "say Seoul, South Korea, %s, and  %s, %s, %s, Sir",
                     todayWInfo[0].c_str(), todayWInfo.back().c_str(), tonightWInfo[0].c_str(), tonightWInfo.back().c_str());
            /* output info */
            cout << "    " << todayWInfo[0] << "." << todayWInfo.back() << "." << endl;
            cout << "    " << tonightWInfo[0] << "." << tonightWInfo.back() <<" Sir."<< endl;
            break;
        default:
            break;
    }
    
    /* pipe open to say Today or Tonight - ... */
    pipeOpen(popenParam);
    /* clear vector */
    todayWInfo.clear();
    tonightWInfo.clear();
}


void informTime(){
    
    struct tm *t;
    time_t timer;
    timer = time(NULL);
    t = localtime(&timer);
    
    char popenParam[100];
    char oTime[10];
    FILE * file;
    
    int hour = t->tm_hour;
    if (t->tm_min < 10){
        sprintf(oTime, "%d:0%d", hour, t->tm_min );
    }else {
        sprintf(oTime, "%d:%d", hour, t->tm_min );
    }
    
    
    cout << "Jasfom speaking..." << endl;
    
    if (hour <5)
    {
        cout << "   It's " << oTime  <<" AM. Sir, you have to sleep now." << endl;
        sprintf(popenParam, "say Its %d %d AM. Sir, you have to sleep now.", hour, t->tm_min);
        pipeOpen(popenParam);
        
    }else if(hour < 12)
    {
        cout << "   It's " << oTime  <<" AM. Good morning, Sir. It is Great day." << endl;
        sprintf(popenParam, "say Its %d, %d AM. Good morning, Sir. It is Great day.", t->tm_hour, t->tm_min);
        pipeOpen(popenParam);
    }
    else if(hour >= 12)
    {
        cout << "   It's " << oTime  <<" PM. Good afternoon, Sir. Enjoy your day." << endl;
        sprintf(popenParam, "say Its %d %d PM. Good afternoon, Sir. Enjoy your day.", hour, t->tm_min);
        pipeOpen(popenParam);
    }
    else
    {
        cout << "   It's " << oTime  <<" AM. Sir, Today is the first day of the rest of your life." << endl;
        sprintf(popenParam, "say Its %d %d AM. Sir, Today is the first day of the rest of your life.", t->tm_hour, t->tm_min);
        pipeOpen(popenParam);
    }
}

void readyPintos(){
    pipeOpen("open -a Sublime\\ Text\\ 2.app");
    pipeOpen("open -a Terminal.app");
    pipeOpen("open /Users/jowon/Dropbox/OS/Pintos.pdf");
    cout << "Jasfom speaking..." << endl;
    cout << "   Now you can do pintos programming. Sir." << endl;
    pipeOpen("say Now you can do pintos programming. Sir.");

}

void outputMacInfo(){
    cout << "\nJasfom Speaking..." << endl;
    cout << "   This imformations are about your MacBook Air. Sir\n" << endl;
    cout << pipeOpen("scutil --get ComputerName;", "r");
    cout << pipeOpen("sysctl -n hw.memsize | awk '{print $0/1073741824\"GB RAM\"}';", "r");
    cout << pipeOpen("sw_vers | awk -F':\t' '{print $2}' | paste -d ' ' - - -;", "r");
    cout << pipeOpen("df -Hl | grep 'disk1' | awk '{print $4\"B/\"$2\"B free (\"$5\" used)\"}'", "r");
    pipeOpen("say This imformations are about your MacBook Air. Sir");

}
void man(){
    cout << "Jasfom speaking..." << endl;
    cout << "	This is manuals of me. Sir" << endl;
    cout << " ------------------------------------- " << endl;
    cout << "	jasfom who" << endl;
    cout << "	jasfom exit" << endl;
    cout << "	jasfom pintos" << endl;
    cout << "	jasfom weather or wt" << endl;
    cout << "	jasfom time" << endl;
    cout << "	jasfom amac" << endl;
    cout << " ------------------------------------- " << endl;
    pipeOpen("say This is manuals of me. Sir");
}

void exitMac() {
    cout << "Jasfom speaking..." << endl;
    cout << "   Do you really want to shutdown MacBook? (y/n)" << endl;
    pipeOpen("say Do you really want to shutdown MacBook?");
    char ch;
    cin>>ch;
    if (ch == 'y')
        pipeOpen("sudo shutdown -h +0");
    else if(ch == 'n')
        cout << "   Canceled!   " <<endl;
}
