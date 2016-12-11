#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
using namespace std;

vector<string> split_by_space(string in){
	int base = 0;
	vector<string> ret;
	for(int i = 0; i < in.length(); i++){
		if(in[i] == ' '){
			ret.push_back(in.substr(base, i - base));	
			base = i + 1;
		}
	}
	return ret;
}
int build_database(char* filename, map<int, map<string, int> > &freq_map, map<int, int> &count_map){
	ifstream input;
	input.open(filename);
	if (!input.is_open()){
		cout << "error: in reading input file \"" << filename << "\"" << endl;
		return 0;
	}
	string info;
	getline(input, info);
	vector<string> infovec = split_by_space(info);
	int total_sample = stoi(infovec[0]);

	for(int i = 0; i < total_sample; i++){
		string label;
		string question;
		getline(input, label);
		getline(input, question);

		vector<string> label_vec = split_by_space(label);
		vector<string> question_vec = split_by_space(question);	

		for(int j = 0; j < label_vec.size(); j++){
			int label_index = stoi(label_vec[j]);
			if (count_map.find(label_index) != count_map.end()){
				count_map[label_index] += 1;
			}
			else{
				count_map[label_index] = 1;
			}
			for(int k = 0; k < question_vec.size(); k++){
				if (freq_map[label_index].find(question_vec[k]) != freq_map[label_index].end()){
					freq_map[label_index][question_vec[k]] += 1;
				}
				else{
					freq_map[label_index][question_vec[k]] = 1;
				}
			}
			/*
			if (newlabelFlag){
				freq_map[label_index] = new map<string, int>();
			}
			map<string, int> cur_label_map = *(freq_map[label_index]);
			for(int k = 0; k < question_vec.size(); k++){
				if (cur_label_map.find(question_vec[k]) != cur_label_map.end()){
					cur_label_map[question_vec[k]] += 1;
				}
				else{
					cur_label_map[question_vec[k]] = 1;
				}
			}
			cout << cur_label_map.size() << endl;
			*/
			
		}
	}
	return 1;
}
int main(int argc, char** argv){
	if (argc < 3){
		cout << "usage: ./nb [input_file] [output_file]" << endl;
		return 1;
	}
	char* input_file_name = *(argv + 1);
	char* output_file_name = *(argv + 2);
	
	map<int, map<string, int> > freq_map;
	map<int, int> count_map;
	if (!build_database(input_file_name, freq_map, count_map)){
		return 1;
	}
	cout << count_map.size() << endl;
	cout << freq_map[4].size() << endl;



	return 0;
}
