#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
using namespace std;

bool not_equal(string str, vector<string> str_arr){
	int i = 0;
	while(i < str_arr.size()){
		if(str == str_arr[i])
			return false;
		++i;
	}
	return true;
}

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

struct obj{

	vector<double> att;
	string type;

	obj(vector<double> &att, string &type){
		this->att = att;
		this->type = type;
	}

	void print(){
		int i = 0;
		while(i < att.size()){
			cout<<att[i]<<',';
			++i;
		}
		cout<<type<<endl;
	}
};

vector<string> find_types(vector<obj> arr_obj){ //finds type names: setosa,versicolor. Can be altered to find more than 2 types(via remember) but according to Karol we deal with 2 types max.
						//returns null vector if array is size 0 or else returns 1 or 2 unique types.
	if(arr_obj.size() == 0){
		return vector<string>();
	}

	vector<string> unique;
	unique.push_back(arr_obj[0].type);

	int i = 0;
	while(i < arr_obj.size()){
		if(arr_obj[0].type != arr_obj[i].type){
			unique.push_back(arr_obj[i].type);
			break;
		}
		++i;
	}

	return unique;
}

void extract(vector<obj> &arr_obj, string &str, unsigned int &count_comma){

	string temp_s;
	vector<double> temp_a;

	int i = 0;
	for(int j = 0;j < count_comma;++j){
		string temp;
		while(str [i]!= ','){
			temp.push_back(str[i]);
			++i;
		}
		temp_a.push_back(stod(temp));
		++i;
	}
	temp_s = str.substr(i, (str.size()-i+1));

	arr_obj.push_back(obj(temp_a, temp_s));

}

struct node{
	vector <obj> arr_obj;
	node* left;
	node* right;
	string decision;

	node (vector<obj> arr_obj, string decision){
		this->arr_obj = arr_obj;
		left = nullptr;
		right = nullptr;
		this->decision = decision;
	}

	double entropy(vector<obj> arr_obj){
		vector<string> unique_types = find_types(arr_obj);
		if(unique_types.size() == 1){ //homogenous
			return 0;
		}

		if(unique_types.size() == 0){//empty
			return 0;
		}

		int i = 0;
		double no = 0,yes = 0;
		while(i < arr_obj.size()){
			if(arr_obj[i].type == unique_types[0])
				++no;
			else if(arr_obj[i].type == unique_types[1])
				++yes;
			++i;
		}
		double p_no = no/(no+yes);
		double p_yes = yes/(no+yes);
		double entropy = -(p_no*log2(p_no))-(p_yes*log2(p_yes));

		return entropy;
	}

	vector<double> find_splitp(){//find split point
		double gain = 0, cutoff = 0, att_cutoff = 0;
		vector<obj> smaller,greater;
		int i = 0, j = 0;
		while(i < arr_obj[0].att.size()){
			j = 0;
			while(j < arr_obj.size()){
				double temp_cutoff = arr_obj[j].att[i];
				int k = 0;
				while(k < arr_obj.size()){
					if(arr_obj[k].att[i] <= temp_cutoff){
						smaller.push_back(arr_obj[k]);
					}
					else if(arr_obj[k].att[i] > temp_cutoff){
						greater.push_back(arr_obj[k]);
					}
					++k;
				}
				if(temp_cutoff == 1.9){
					int i =0;
					while(i < greater.size()){ greater[i].print(); ++i;}
				}
				double pe1 = entropy(arr_obj);
				double pe2 = entropy(smaller);
				double pe3 = entropy(greater);

				double size_smaller = smaller.size();
				double size_greater = greater.size();
				double size_all = arr_obj.size();
				double temp_gain = pe1 - ((size_smaller/size_all)*pe2) - ((size_greater/size_all)*pe3);
				if(temp_gain > gain){
					cutoff = temp_cutoff;
					att_cutoff = i;
					gain = temp_gain;
				}
				smaller.clear();
				greater.clear();
				++j;
			}
			++i;
		}
		cout<<gain<<endl;
		return vector<double>{cutoff,att_cutoff};
	}

	node* split(node* circle){
		if(find_types(circle->arr_obj).size() < 2){
			return nullptr;
		}

		vector<double> splitp = circle->find_splitp(); // index 0 cutoff , index 1 attribute
		cout<<splitp[0]<<splitp[1]<<endl;
		vector<obj> smaller,greater;
		int i = 0;
		while(i < circle->arr_obj.size()){
			double cutoff = splitp[0];
			if(circle->arr_obj[i].att[static_cast<int>(splitp[1])] <= splitp[0]){
				smaller.push_back(circle->arr_obj[i]);
			}
			else if(circle->arr_obj[i].att[static_cast<int>(splitp[1])] > splitp[0]){
				greater.push_back(circle->arr_obj[i]);
			}
			++i;
		}
		string s_ltemp = ("Attribute " + to_string(splitp[1]) + " <= " + to_string(splitp[0]) );
		string s_rtemp = ("Attribute " + to_string(splitp[1]) + " > " + to_string(splitp[0]) );

		circle->left = new node(smaller, s_ltemp);
		circle->right = new node(greater, s_rtemp);
//		cout<<"SIZE OF SMALLER "<<smaller.size()<<" SIZE GREATER "<<greater.size()<<endl;
		circle->left = split(circle->left);
		circle->right = split(circle->right);
	}


};

int main(int argc,char **argv){
	ifstream file(argv[1], ifstream::in);

	unsigned int comma_count = 0;
	vector<obj> arr_obj;

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
			}
				extract(arr_obj, line, comma_count);// very important, extracts the data from the file into objects.
		}
	}
	cout<<"OBJECT LENGTH"<<arr_obj.size()<<endl;

	node* root = new node(arr_obj, "root has no decision");

	vector<double> split_p = root->find_splitp();
	cout<<"CUTOFF "<< split_p[0] <<", ATTRIBUTE AT CUTOFF "<< split_p[1] <<endl;

	//root->split(root);
	return 0;
}
