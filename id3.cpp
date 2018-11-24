#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int count_att(string str){
	int i = 0;
	int count = 0;

	if(str.size() == 0)
		throw -1;

	while(i < str.size()){
		if(str[i] == ','){
			++count;
		}

		++i;
	}
	return count;
}

void extract(vector<vector<int>> &att, vector<vector<char>> &type, string str, int count_comma, int row_at){

	int i = 0;
	int column = 0;
	while(i < (2 * count_comma)){
		if(str[i] != ','){
			att[column].push_back(str[i]);
			++i;
			++column;
		}
		else
			++i;
	}

	i = (2 * count_comma);
	type.push_back(vector<char>());
	while(i < str.size()){
		type[row_at].push_back(str[i]);
		++i;
	}
}

int main(int argc,char **argv){
	ifstream file(argv[1], ifstream::in);

	int comma_count = 0;
	int row_at = 0;
	vector<vector<int>> att;
	vector<vector<char>> type;

	if(!file.is_open()){
		printf("Could not open the file\n");
	}
	else{
		string line;
		bool done = false;

		while(getline(file,line)){
			if(!done){
				comma_count = count_att(line);
				cout<<"commacount "<<comma_count<<endl;
				done = true;
				att = vector<vector<int>>(comma_count);

			}
				extract(att, type, line, comma_count, row_at);
				++row_at;
		}
	}

	cout<<"Printing attribute vector size "<< att.size()<<endl;
	cout<<"Printing attribute vector index 0 size "<<att[0].size()<<endl;
	cout<<"Printing text vector size "<<type.size()<<endl;
	cout<<"Printing text vector index 0 size "<<type[0].size()<<endl;
	return 0;
}
