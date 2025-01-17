#include "tempTrender.h"

#include <fstream>
#include <map>
#include <stdlib.h>
#include <TF1.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLine.h>

#include <TH1.h> // 1d histogram classes
#define NB_MAX_FILE_VALUES 200000
#define MAX_YEAR 2100 // maximum year of input data
#define MIN_TEMP_VALUE -100 // minimum value of temperature
#define ARRAY_SIZE 100000 // minimum value of temperature

/*
 * Constructor
 */
tempTrender::tempTrender(std::string filePath) {
        // Store file path parameter into member variable
        _filePath = filePath;
        
        //cout << "The user supplied " << filePath << " as the path to the data file." << endl;
        //cout << "You should probably store this information in a member variable of the class. Good luck with the project! :)" << endl;
}


//////////////////////////////////
// JASMAIN CODE PART
//////////////////////////////////

/* Function hottest temp per year
 * @param vector of strings
 *               with following format "%s %s %s %s %s" 
 *               (year, month, day, temp, temp with urban correction, city)
 * @return map of mean temp per year
 */
std::map<int, double> coldTempPerYear(std::vector<std::string> entries) {
        // temporary values
        std::string year_str, value_str;
        size_t pos;
        int year;
        double value;
        
        // storage
        double cold_values[MAX_YEAR]={0};
        
        // for each entry, extract year and value
        for(size_t y=0; y<entries.size(); y++) {
                std::string s = entries[y];
                // get year from 4 first characters
                year_str = s.substr(0, 4);
                year = atoi(year_str.c_str());                             
                
                // get value from 5th field
                for(size_t i=0; i<4; i++) {
                        pos = s.find(' '); // find first delimiter
                        s.substr(0, pos);
                        s.erase(0, pos + 1);
                }
                pos = s.find(' ');
                value_str = s.substr(0, pos);
                value = atof(value_str.c_str());
                
                // add value to arrays
                if (value < cold_values[year] ) {
                    cold_values[year] = value;
                }
        }
        
        // return variable
        std::map<int, double> ret;
        double cold;
        
        // for each year, compute mean value
        for(size_t i=0; i<MAX_YEAR; i++) {
               cold = cold_values[i];
               std::cerr << "year : " << i << " coldest temp : " << cold << std::endl;
               ret.insert(std::make_pair<int, double>((int)i, (double)cold));
        }
        return ret;
}
void tempTrender::coldestTempPerYear() {
        
        // open input file
        std::string line;
        ifstream inputfile(_filePath.c_str());
        
        
        // create array to store values
        std::vector<std::string> entries;
        
        
        // read values and stor it into array, hist or something else
        if(inputfile.is_open()) {
                while(getline(inputfile, line)) {
                        entries.push_back(line);
                }
        }

        std::map<int, double> cold = coldTempPerYear(entries);
        
        // create canvas for graph
        TCanvas *c2 = new TCanvas("Jasmain", "Project : COldest temperature over each year");

        // create new histogram object
        TH1D* hist2 = new TH1D("hist", "Coldest Temp Per Year", cold.size(), 1722, 2013);
        
        for( std::map<int, double>::iterator it = cold.begin();
             it != cold.end();
             it++ ) {
                // fill hist with date and value from mean temp per year map
                //std::cerr << "value : " << it->second << std::endl;
                hist2->Fill(it->first, it->second);
        }
        //Axis title
        hist2->SetTitle("Coldest temperature over each year (Uppsala, 1722-2013)");
        hist2->GetXaxis()->SetTitle("Year");
        hist2->GetXaxis()->CenterTitle();
        hist2->GetYaxis()->SetTitle("Coldest Temperature (Celsius deg)");
        hist2->GetYaxis()->CenterTitle();

        // draw hist
        hist2->Draw("SAME");
}
std::map<int, double> hotTempPerYear(std::vector<std::string> entries) {
        // temporary values
        std::string year_str, value_str;
        size_t pos;
        int year;
        double value;
        
        // storage
        double hot_values[MAX_YEAR]={0};
        
        // for each entry, extract year and value
        for(size_t y=0; y<entries.size(); y++) {
                std::string s = entries[y];
                // get year from 4 first characters
                year_str = s.substr(0, 4);
                year = atoi(year_str.c_str());                             
                
                // get value from 5th field
                for(size_t i=0; i<4; i++) {
                        pos = s.find(' '); // find first delimiter
                        s.substr(0, pos);
                        s.erase(0, pos + 1);
                }
                pos = s.find(' ');
                value_str = s.substr(0, pos);
                value = atof(value_str.c_str());
                
                // add value to arrays
                if (value > hot_values[year] ) {
                    hot_values[year] = value;
                }
        }
        
        // return variable
        std::map<int, double> ret;
        double hot;
        
        // for each year, compute mean value
        for(size_t i=0; i<MAX_YEAR; i++) {
               hot = hot_values[i];
               //std::cerr << "year : " << i << " hottest temp : " << hot << std::endl;
               ret.insert(std::make_pair<int, double>((int)i, (double)hot));
        }
        return ret;
}
void tempTrender::hottestTempPerYear() {
        
        // open input file
        std::string line;
        ifstream inputfile(_filePath.c_str());
        
        
        // create array to store values
        std::vector<std::string> entries;
        
        
        // read values and stor it into array, hist or something else
        if(inputfile.is_open()) {
                while(getline(inputfile, line)) {
                        entries.push_back(line);
                }
        }

        std::map<int, double> hot = hotTempPerYear(entries);
        
        // create canvas for graph
        TCanvas *c1 = new TCanvas("Jasmain", "Project : Hottest temperature over each year");

        // create new histogram object
        TH1D* hist = new TH1D("hist", "Hottest Temp Per Year", hot.size(), 1722, 2013);
        
        for( std::map<int, double>::iterator it = hot.begin();
             it != hot.end();
             it++ ) {
                // fill hist with date and value from mean temp per year map
                //std::cerr << "value : " << it->second << std::endl;
                hist->Fill(it->first, it->second);
        }
        
        
        //Axis title
        hist->SetTitle("Hottest temperature over each year (Uppsala, 1722-2013)");
        hist->GetXaxis()->SetTitle("Year");
        hist->GetXaxis()->CenterTitle();
        hist->GetYaxis()->SetTitle("Hottest Temperature (Celsius deg)");
        hist->GetYaxis()->CenterTitle();

        // draw hist
        hist->Draw("SAME");
}


//////////////////////////////////
// JOHAN CODE PART
//////////////////////////////////


void tempTrender::tempOnDay(int monthToCalculate, int dayToCalculate) {

    ifstream file(_filePath.c_str());
    
    //Defining all variables we need
    vector<string> entries;
    float temp[ARRAY_SIZE];
    
    string line, month_str, day_str, temp_str;
    int i, j=0, size, pos, month, day;
    float t, mean=0, StanDev=0;
    
    //Check if the file is opened
    if(file.is_open()) 
    {
        //Fill up the vector with the entries
        while(getline(file, line)) 
        {
            entries.push_back(line);
        }
    }
    
    size = entries.size();
    

    
    
    TCanvas *c1= new TCanvas("Johan","Project : Temperature on a given day");
    
    //creating histogram
    TH1D* hist = new TH1D("Hist", "Temperature on a given day", 100, -20, 40);
    
       //goes through all the entries
    for(i=0; i<size; i++)
    {
        line = entries[i];
        
        //get the month from 2nd field               
        pos = line.find(' '); 
        line.erase(0, pos + 1);
        pos = line.find(' ');
        month_str = line.substr(0, pos);
        month = atoi(month_str.c_str());
        
        //get the day from 3rd field               
        pos = line.find(' '); 
        line.erase(0, pos + 1);
        pos = line.find(' ');
        day_str = line.substr(0, pos);
        day = atoi(day_str.c_str());
        
        
        if ((month == monthToCalculate) && (day == dayToCalculate))
        {
            //get the corresponding temperature at the 4th field
            pos = line.find(' '); 
            line.erase(0, pos + 1);
            pos = line.find(' ');
            temp_str = line.substr(0, pos);
            t = atof(temp_str.c_str());
            mean=mean+t;
            temp[j]=t;
            j=j+1;
            hist->Fill(t);
        }
    }
    
    
    //Calculate the mean value on that day
    mean=mean/j;
    
    //Calculate the standart deviation
    for (i=0; i<=j; i++){
        StanDev = StanDev+(temp[i]-mean)*(temp[i]-mean)/(j+1);
    }
    StanDev = sqrt(StanDev);
    
    cout << "The mean value on this day is : " << mean << endl;
    cout << "The standart deviation is : " << StanDev << endl;
    
    //Setting up X and Y axis
    hist->GetYaxis()->SetTitle("Entries");
    hist->GetXaxis()->SetTitle("Temperature in Celsius (deg)");
        

    //drawing histogram
    hist->SetFillColor(kGreen);
    hist->Draw("SAME");
    

}




//////////////////////////////////
// ESTELLE CODE PART
//////////////////////////////////

/* Function mean temp per year
 * @param vector of strings
 *               with following format "%s %s %s %s %s" 
 *               (year, month, day, temp, temp with urban correction, city)
 * @return map of mean temp per year
 */
std::map<int, double> meanTempPerYear(std::vector<std::string> entries) {
        
        // we assume that values are uniformatically spread on the year
        
        // temp values
        std::string year_str, value_str;
        size_t pos;
        int year;
        double value;
        
        // storage
        int nb_values[MAX_YEAR]={0};
        double sum_values[MAX_YEAR]={0};
        
        // for each entry, extract year and value
        for(size_t y=0; y<entries.size(); y++) {
                
                std::string s = entries[y];
                
                // get year from 4 first characters
                year_str = s.substr(0, 4);
                year = atoi(year_str.c_str());
                
                // get value from 5th field
                for(size_t i=0; i<4; i++) {
                        pos = s.find(' '); // find first delimiter
                        s.substr(0, pos);
                        s.erase(0, pos + 1);
                }
                pos = s.find(' ');
                value_str = s.substr(0, pos);
                value = atof(value_str.c_str());
                // add value to arrays
                nb_values[year]++;
                sum_values[year]+=value;
        }
        
        // return variable
        std::map<int, double> ret;
        double mean;
        
        // for each year, compute mean value
        for(size_t i=0; i<MAX_YEAR; i++) {
                if(nb_values[i]>0) {
                        mean = sum_values[i]/nb_values[i];
                        std::cerr << "year : " << i << " mean : " << mean << std::endl;
                        ret.insert(std::make_pair<int, double>((int)i, (double)mean));
                }
        }
        
        return ret;
}

void tempTrender::tempPerYear(int yearToExtrapolate) {
        
        // open input file
        std::string line;
        ifstream inputfile(_filePath.c_str());
        
        // create array to store values
        std::vector<std::string> entries;
        
        // read values and store into entries array
        if(inputfile.is_open()) {
                while(getline(inputfile, line)) {
                        entries.push_back(line);
                }
        }
        
        // compute mean per year
        std::map<int, double> meanPerYear = meanTempPerYear(entries);
        
        // compute mean for all time
        double meanAllTime = 0;
        for( std::map<int, double>::iterator it = meanPerYear.begin();
             it != meanPerYear.end();
             it++ ) {
                 meanAllTime += it->second;
         }
         meanAllTime /= meanPerYear.size();
         
         std::cout << "meanAllTime = " << meanAllTime << endl;
        
        
        // create canvas for graph
        TCanvas *c1 = new TCanvas("Estelle", "Project : Mean Temp Per Year");
        
        // create new histogram object

        TH1F* hist = new TH1F("graph", "Mean Temp Per Year", meanPerYear.size(), 1722, 2013);

        // fill hist with mean temp per year values from input file
        for( std::map<int, double>::iterator it = meanPerYear.begin();
             it != meanPerYear.end();
             it++ ) {
                // fill hist with date and value from mean temp per year map
                //std::cerr << "value : " << it->second << std::endl;
                hist->Fill(it->first, it->second);
        }
        
        // Draw horizontal mean
        TLine *meanline = new TLine (1722,meanAllTime,2013,meanAllTime);
        
        // This code is given from project instruction for creating the graph

        TGraph* graph = new TGraph();
        
        for(int bin = 1; bin < hist->GetNbinsX(); ++bin) {
            graph->Expand(graph->GetN() + 1, 100);
            graph->SetPoint(graph->GetN(), hist->GetBinCenter(bin),
                hist->GetBinContent(bin));
        }

        // create function for extrapolation
        TF1 *f = (TF1*)hist->GetFunction("pol7");
        //f->Eval(yearToExtrapolate);

        // mean value printed
        gStyle->SetOptStat("m"); // Note : doesn't work :(

        //Axis title
        hist->SetTitle("Mean temperature per year (Uppsala, 1722-2013)");
        hist->GetXaxis()->SetTitle("Year");
        hist->GetXaxis()->CenterTitle();
        hist->GetYaxis()->SetTitle("Mean temperature (Celcius Deg)");
        hist->GetYaxis()->CenterTitle();
        
        // draw hist
        hist->Draw("SAME");

        // draw mean line
        meanline->Draw();
}
