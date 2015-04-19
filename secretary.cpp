/* This Created By Jowon. */
/* This is named jasfom means just a smart friend of me. */
/* This is Jowon's Personal secretary. */
/* Jowon Think this as a friend. */
/* Jasfom was born 2015. 4. 17 */


#include <iostream>
#include <vector>
#include <cstdio>
#include <time.h>
#include <unistd.h>

using namespace std;

void informWeather() {
    
    // today weather information
    vector<string> todayWInfo;
    // tonight weather information
    vector<string> tonightWInfo;
    
    char buffer[200];
    char popenParam[100];
    
    /* TODAY procedure */
    /* pipe open to get TODAY's weather infomation, and save to buffer */
    FILE *file = popen("curl -s https://weather.yahoo.com/south-korea/seoul/seoul-1132599/ | awk '/Today -/' | textutil -convert txt -stdin -stdout -format html", "r");
    
    fgets(buffer, 200, file);
    pclose ( file );
    
    // for tokenizing
    char * token;
    /* tokenizing procedure */
    token = strtok (buffer, ".");
    string tempForToken;
    
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
    file = popen("curl -s https://weather.yahoo.com/south-korea/seoul/seoul-1132599/ | awk '/Tonight -/' | textutil -convert txt -stdin -stdout -format html", "r");
    
    fgets(buffer, 200, file);
    pclose ( file );
    
    /* tokening procedure */
    token = strtok (buffer, ".");
    
    /* tokenize and push it to tonight's vector */
    while (token != NULL)
    {
        tempForToken = token;
        tonightWInfo.push_back (tempForToken);
        tempForToken.clear();
        token = strtok (NULL, ".");
    }
    token = NULL;
    
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
            snprintf(popenParam, 200, "say %s, and  %s,  Sir",
                     todayWInfo[0].c_str(), tonightWInfo[0].c_str());
            /* output info */
            cout << "    " << todayWInfo[0] << "." << endl;
            cout << "    " << tonightWInfo[0] << "." << " Sir."<< endl;
            break;
            
            /* Just today has information about chanc of rain. */
        case 1:
            snprintf(popenParam, 200, "say %s, and  %s, %s,  Sir",
                     todayWInfo[0].c_str(), todayWInfo.back().c_str(), tonightWInfo[0].c_str());
            /* output info */
            cout << "    " << todayWInfo[0] << "." << todayWInfo.back().c_str() << "." << endl;
            cout << "    " << tonightWInfo[0] << "." << " Sir."<< endl;
            
            break;
            
            /* Just tonight has information about chanc of rain. */
        case 2:
            snprintf(popenParam, 200, "say %s, and  %s, %s, Sir",
                     todayWInfo[0].c_str(), tonightWInfo[0].c_str(), tonightWInfo.back().c_str());
            /* output info */
            cout << "    " << todayWInfo[0] << "." << endl;
            cout << "    " << tonightWInfo[0] << "." << tonightWInfo.back() <<" Sir."<< endl;
            break;
            
            /* Both has information about chance of rain. */
        case 3:
            snprintf(popenParam, 200, "say Seoul, South Korea, %s, and  %s, %s, %s, Sir",
                     todayWInfo[0].c_str(), todayWInfo.back().c_str(), tonightWInfo[0].c_str(), tonightWInfo.back().c_str());
            /* output info */
            cout << "    " << todayWInfo[0] << "." << todayWInfo.back() << "." << endl;
            cout << "    " << tonightWInfo[0] << "." << tonightWInfo.back() <<" Sir."<< endl;
            
            break;
        default:
            break;
    }
    
    /* pipe open to say Today or Tonight - ... */
    file = popen ( popenParam, "r" );
    pclose ( file );
    
    /* clear vector */
    todayWInfo.clear();
    tonightWInfo.clear();
}


void informTime(){
    
    struct tm *t;
    time_t timer;
    timer = time(NULL);
    t = localtime(&timer);
    
    FILE * file;
    int hour = t->tm_hour;
    
    char popenParam[100];
    
    if (hour <5)
    {
        cout << "Jasfom speaking..." << endl;
        cout << "   It is " << t->tm_hour << ":" << t->tm_min  <<" AM. Sir, you have to sleep now." << endl;
        sprintf(popenParam, "say It is %d : %d A m. Sir, you have to sleep now.", t->tm_hour, t->tm_min);
        file = popen (popenParam, "r");

        pclose(file);
        
    }else if(hour < 12)
    {
        printf("Good morning, Sir. It is Great day.\n");
        cout << "Jasfom speaking..." << endl;
        cout << "   It is " << t->tm_hour << ":" << t->tm_min  <<" AM. Good morning, Sir. It is Great day." << endl;
        sprintf(popenParam, "say It is %d : %d A m., Good morning, Sir. It is Great day.", t->tm_hour, t->tm_min);
        file = popen (popenParam, "r");

        pclose(file);
    }
    else if(hour >= 12)
    {
        printf("Good afternoon, Sir. Enjoy your day.\n");
        cout << "Jasfom speaking..." << endl;
        cout << "   It is " << t->tm_hour << ":" << t->tm_min  <<" PM. Good afternoon, Sir. Enjoy your day." << endl;
        sprintf(popenParam, "say It is %d : %d P m. Good afternoon, Sir. Enjoy your day.", t->tm_hour, t->tm_min);
        file = popen (popenParam, "r");
        pclose (file);
    }
    else
    {
        printf("Sir, Today is the first day of the rest of your life.\n");
        cout << "Jasfom speaking..." << endl;
        cout << "   It is " << t->tm_hour << ":" << t->tm_min  <<" AM. Sir, Today is the first day of the rest of your life." << endl;
        sprintf(popenParam, "say It is %d : %d A m. Sir, Today is the first day of the rest of your life.", t->tm_hour, t->tm_min);
        file = popen (popenParam, "r");
        pclose(file);
        
    }
}


int main(int argc, const char*argv[])
{
    string args;
    string parameters[6] = {"who", "pintos", "weather", "wt", "time"};
    
    if (argc > 1){
        
        /* Human friendly message.
        for (int i=0; i<argc-1; i++){
            args += argv[i+1];
            args += " ";
        }*/
        
        /* when "who" command */
        if (!(strcmp(argv[1], parameters[0].c_str())))
        {
            cout << "Jasfom speaking..." << endl;
            popen("say I am your secretary Jasfom, Sir.","r");
            cout << "   I am your secretary Jasfom, Sir." << endl;
        }

        /* when "pintos" command */
        if(!(strcmp(argv[1], parameters[1].c_str())))
        {
            popen("say You need to do pintos study. Sir.", "r");
            cout << "Jasfom speaking..." << endl;
            cout << "   You need to do pintos study. Sir." << endl;
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
        popen("say What can I help you?, Sir.","r");
        cout << "Jasfom speaking..." << endl;
        cout << "   What can I help you?, Sir." << endl;

    }
    return 0;
}


