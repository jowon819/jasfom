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

/* To execute command */
bool pipeOpen(const char * comm){
    FILE * file = popen(comm, "r");
    
    if (file == NULL)
        return false;
    pclose(file);
    return true;
}

FILE * pipeOpen(const char *comm) {

    fgets (buffer, STRINGSIZE, file);
    
}

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


int main(int argc, const char*argv[])
{
    string args;
    string parameters[6] = {"who", "pintos", "weather", "wt", "time"};
    
    if (argc > 1){

        /* when "who" command */
        if (!(strcmp(argv[1], parameters[0].c_str())))
        {
            cout << "Jasfom speaking..." << endl;
            cout << "   I am your secretary Jasfom, Sir." << endl;
            pipeOpen("say I am your secretary Jasfom, Sir.");
        }
        
        /* when "pintos" command */
        if(!(strcmp(argv[1], parameters[1].c_str())))
        {
            cout << "Jasfom speaking..." << endl;
            cout << "   You need to do pintos study. Sir." << endl;
            pipeOpen("say You need to do pintos study. Sir.");
        }
        
        /* when "wt or weather" command */
        if (!(strcmp(argv[1], parameters[2].c_str())) || !(strcmp(argv[1], parameters[3].c_str()))){
            informWeather();
        }
        
        /* when "time" command */
        if (!(strcmp(argv[1], parameters[4].c_str()))){
            informTime();
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




