#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <SDL.h>

#define path "D:\\data.txt"			//Edit the path here. Replace D:\\data.txt with your path.

using namespace std;
constexpr int width = 800;
constexpr int height = 600;
SDL_Renderer* r;
SDL_Window* window;

bool isNumber(string s);
void SDLInit();
int SDLQuit();

int main(int argc, char** args)
{
	while (true)
	{
		cout << "What would you like to do? Type the number.\n1. Input Scores\n2. View Progress\n3. Edit Data\n4. Exit" << endl;
		int choice;
		cin >> choice;
		if (choice == 4)
			break;
		else if (choice == 1)
		{
			ofstream outfile;
			outfile.open(path, ios::app);
			ifstream infile;
			infile.open(path);

			// Listing the current categories.
			cout << "What category are you inputting scores for? (Enter the Number)" << endl;
			string line;
			int count = 1;
			while (getline(infile, line))
			{
				int x = line.find(':');
				cout << count << ". " << line.substr(0, x) << endl;
				count++;
			}
			cout << count << ". " << "Create New Category" << endl;
			count++;
			cout << count << ". " << "Exit" << endl;
			int c;
			cin >> c;
			if (c > count || c < 1)
				cout << "Sorry, command not recognized." << endl;
			else if (c == count)
				break;
			else if (c == count - 1)
			{
				cout << "Enter the name of the new category or type \"cancel\" to cancel." << endl;
				string input;
				cin >> ws;
				getline(cin, input);
				if (input.compare("cancel") != 0)
				{
					if(line.compare("") != 0)						//Not the first category in the file
						outfile << "\n" << input << ":";
					else
						outfile << input << ":";					//Handles creating the first category
					cout << "Type in scores one by one or as a comma separated list (1,2,3) or (1, 2, 3). Type \"end\" when finished. If switching days, start a new line and type \"|\"." << endl;
					while (true)
					{
						string tempin;
						cin >> ws;
						getline(cin, tempin);
						tempin.erase(remove(tempin.begin(), tempin.end(), ' '), tempin.end());
						if (tempin.compare("end") != 0)
						{
							stringstream ss(tempin);
							string tempss;
							while (getline(ss, tempss, ','))
							{
								if (!(tempss.compare("|") == 0 && isNumber(tempss.substr(0, tempss.length() - 1))))
									outfile << tempss << ",";
								else
									outfile << tempss;
								/*if (isNumber(tempss))
									outfile << tempss << ",";
								else if ((tempss.compare(".") || tempss.compare("|") == 0) && isNumber(tempss.substr(0, tempss.length() - 1)))
									outfile << tempss;
								else
									cout << "Your input contained non-numerical values (besides commas, spaces, and pipes (\"|\"). Input failed. If using a comma separated list, all values before the non-numerical value was inserted. Please start at the error." << endl;*/
							}
						}
						else
						{
							outfile << "|";
							break;
						}
					}
				}
				infile.close();
				outfile.close();
			}
			else
			{
				//Go to the inputted line, and read in scores same as method above.
				vector<string> data;
				ifstream infile;
				infile.open(path);
				string temp;
				while (getline(infile, temp))
				{
					data.push_back(temp);
				}
				if (data[c - 1].back() != ',' && data[c - 1].back() != '|')
					data[c - 1] += ",";
				cout << "Type in scores one by one or as a comma separated list (1, 2, 3). Type \"end\" when finished. If switching days, start a new day and type \"|\"." << endl;
				while (true)
				{
					string tempin;
					cin >> ws;
					getline(cin, tempin);
					tempin.erase(remove(tempin.begin(), tempin.end(), ' '), tempin.end());
					if (tempin.compare("end") != 0)
					{
						stringstream ss(tempin);
						string tempss;
						while (getline(ss, tempss, ','))
						{
							if (!(tempss.compare("|") == 0 && isNumber(tempss.substr(0, tempss.length() - 1))))
								data[c-1] += tempss += ",";
							else
								data[c-1] += tempss;
							/*if (isNumber(tempss))
								data[c - 1] += tempss += ",";
							else if ((tempss.compare(".") == 0 || tempss.compare("|") == 0) && isNumber(tempss.substr(0, tempss.length() - 1)))
								data[c - 1] += tempss;
							else
								cout << "Your input contained non-numerical values (besides commas, spaces, and pipes (\"|\"). Input failed. If using a comma separated list, all values before the non-numerical value was inserted. Please start at the error." << endl;*/
						}
					}
					else
					{
						data[c - 1] += "|";
						break;
					}
				}
				ofstream outfile;
				outfile.open(path);
				for (unsigned int i = 0; i < data.size() - 1; i++)
					outfile << data[i] << endl;
				outfile << data[data.size() - 1];
			}
			outfile.close();
			infile.close();
		}
		else if (choice == 2)
		{
			ifstream file;
			file.open(path);
			int count = 1;
			string line;
			while (getline(file, line))
			{
				int x = line.find(':');
				cout << count << ". " << line.substr(0, x) << endl;
				count++;
			}
			cout << "Which category would you like to view? Please type the corresponding number." << endl;
			int choice;
			cin >> choice;
			file.close();
			file.open(path);
			for (int i = 0; i < choice; i++)
				getline(file, line);
			int x = line.find(':');
			line = line.substr(x + 1);
			stringstream ss(line);
			string tempss;
			vector<string> days;
			vector<float> avgs;
			while (getline(ss, tempss, '|'))
				days.push_back(tempss);
			for (unsigned int i = 0; i < days.size(); i++)
				if (days[i].compare("") == 0)
					days.erase(days.begin()+i);
			for (unsigned int i = 0; i < days.size(); i++)
			{
				int size = 0;
				float sum = 0.0;
				stringstream ss2(days[i]);
				string t;
				while (getline(ss2, t, ','))
				{
					sum += stof(t);
					size++;
				}
				avgs.push_back(sum / size);
			}
			vector<float> scaledavgs;
			float max = 0.0;
			float min = INT_MAX;
			for (unsigned int x = 0; x < avgs.size(); x++)
			{
				if (avgs[x] > max)
					max = avgs[x];
				if (avgs[x] < min)
					min = avgs[x];
			}
			for (unsigned int x = 0; x < avgs.size(); x++)
				scaledavgs.push_back((avgs[x] - min) / (max - min) * height);
			vector<float> y;
			for (unsigned int x = 0; x < avgs.size(); x++)
				y.push_back(height - scaledavgs[x]);

			//Draw Graph

			SDLInit();
			int xdif = width / (y.size() - 1);
			int xcoord = 0;
			for (unsigned int i = 0; i < y.size(); i++)
			{
				if (i == 0)
				{
					SDL_SetRenderDrawColor(r, 0, 255, 0, 255);
					SDL_RenderDrawPoint(r, xcoord, (int)y[i]);
					SDL_RenderDrawPoint(r, xcoord, (int)y[i] + 1);
					SDL_RenderDrawPoint(r, xcoord, (int)y[i] + 2);

					SDL_RenderDrawPoint(r, xcoord + 1, (int)y[i]);
					SDL_RenderDrawPoint(r, xcoord + 1, (int)y[i] + 1);
					SDL_RenderDrawPoint(r, xcoord + 1, (int)y[i] + 2);

					SDL_RenderDrawPoint(r, xcoord + 2, (int)y[i]);
					SDL_RenderDrawPoint(r, xcoord + 2, (int)y[i] + 1);
					SDL_RenderDrawPoint(r, xcoord + 2, (int)y[i] + 2);
				}
				else
				{
					SDL_SetRenderDrawColor(r, 255, 0, 0, 128);

					SDL_RenderDrawLine(r, xcoord - xdif + 1, (int)y[i - 1] + 1, xcoord, (int)y[i]);

					SDL_SetRenderDrawColor(r, 0, 255, 0, 255);

					SDL_RenderDrawPoint(r, xcoord - 1, (int)y[i] - 1);
					SDL_RenderDrawPoint(r, xcoord - 1, (int)y[i]);
					SDL_RenderDrawPoint(r, xcoord - 1, (int)y[i] + 1);

					SDL_RenderDrawPoint(r, xcoord, (int)y[i] - 1);
					SDL_RenderDrawPoint(r, xcoord, (int)y[i]);
					SDL_RenderDrawPoint(r, xcoord, (int)y[i] + 1);

					SDL_RenderDrawPoint(r, xcoord + 1, (int)y[i] - 1);
					SDL_RenderDrawPoint(r, xcoord + 1, (int)y[i]);
					SDL_RenderDrawPoint(r, xcoord + 1, (int)y[i] + 1);
				}
				xcoord += xdif;
			}
			SDL_RenderPresent(r);
			SDL_Event input;
			bool running = true;
			while (running)
			{
				while (SDL_PollEvent(&input) > 0)
				{
					if (input.type == SDL_QUIT)
						running = false;
				}
			}
			return SDLQuit();

		}
		else if (choice == 3)
		{
			cout << "Not implemented yet." << endl;
			//Edit Data
		}
		else
			cout << "Sorry, command not recognized." << endl;
	}
	return 0;
}

bool isNumber(string s)
{
	for (unsigned int i = 0; i < s.length(); i++)
		if (isdigit(s[i]) == false || s[i] == ',' || s[i] == ',')
			return false;

	return true;
}

void SDLInit()
{
	SDL_CreateWindowAndRenderer(width, height, 0, &window, &r);
	SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
	SDL_RenderClear(r);
	SDL_RenderPresent(r);
}

int SDLQuit()
{
	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
