#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
using namespace std;

struct node{
	vector <vector<double>> att;
	vector <string> type;
	string div_decision;
	node* left;
	node* right;

	node (vector<vector<double>> x, vector<string> y){
		att = x;
		type = y;
	}

	void divide(){
		vector<string> mask;
		double no = 0, yes = 0;
		double t_len = type.size();
		int i = 0;

		while(i < t_len){
			if(type[0] != type[i]){
				mask.push_back(type[0]);
				mask.push_back(type[i]);
			}
			++i;
		}

		cout<<"TYPE NAMES"<<mask[0]<<mask[1]<<endl;

		i = 0;
		while(i < t_len){
			if(type[i] == mask[0])
				++no;
			else if(type[i] == mask[1])
				++yes;
			++i;
		}

		cout<<" NUMBER OF NO "<<no<<" NUMBER OF YES "<<yes<<endl;
		if(mask.size() == 0){
			return;
		}
		double entropy = (-(yes/t_len)*log2(yes/t_len)) - ((no/t_len)*log2(no/t_len));
		cout<<"ENTROPY "<<entropy<<endl;

	}
};


int count_att(string str){
	int i = 0;
	int count = 0;
	if(str.size() == 0)
		throw -1;
	while(i < str.size()){
		if(str[i] == ','){++count;}
		++i;
	}
	return count;
}

void extract(vector<vector<double>> &att, vector<string> &type, string str, int count_comma){

	int i = 0;
	for(int j = 0;j < count_comma;++j){
		string temp;
		while(str [i]!= ','){
			temp.push_back(str[i]);
			++i;
		}
		att[j].push_back(stod(temp));
		++i;
	}
	type.push_back(str.substr(i, (str.size()-i+1) ));
}

int main(int argc,char **argv){
	ifstream file(argv[1], ifstream::in);

	int comma_count = 0;
	vector<vector<double>> att;
	vector<string> type;

	if(!file.is_open()){
		printf("Could not open the file\n");
	}
	else{
		string line;
		bool done = false;

		while(getline(file,line)){
			if(!done){
				comma_count = count_att(line);
				cout<<"Count of commas: "<<comma_count<<endl;
				done = true;
				att.resize(comma_count);

			}
				extract(att, type, line, comma_count);
		}
	}

	cout<<"Printing attribute vector size "<< att.size()<<endl;
	cout<<"Printing attribute vector at index 0 size "<<att[0].size()<<endl;
	cout<<"Printing text vector size "<<type.size()<<endl;
	cout<<"Printing text vector at index 80 size "<<type[80].size()<<endl;

	node* root = new node(att,type);
	root->divide();
	return 0;
}
