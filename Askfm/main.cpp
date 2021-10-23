#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>


using namespace std;

vector<string> READ_FILE(string path)
{
   fstream file_handler(path.c_str());
    string line;
    vector<string>lines;
    while(getline(file_handler,line))
       {
           if(line.size()!=0)
           lines.push_back(line);

       }
       file_handler.close();
       return lines;

}

void WRITE_FILE(string path,vector<string>lines,bool append=true)
{
auto stat=ios::in|ios::out|ios::app;
if(!append)
stat=ios::in|ios::out|ios::trunc;
fstream file_handler(path.c_str(),stat);
for(string line :lines)
{
file_handler<<line<<endl;

}
file_handler.close();

}

vector<string> SPLIT(string str)
{
vector<string>lines;
string s="";
for(int i=0;i<(int)str.size();i++)
{
 if(str[i]==',')
 {
 lines.push_back(s);
 s="";
 }
 else s+=str[i];
}
lines.push_back(s);
return lines;

}

int To_INT(string str)
{  istringstream iss(str);
  int x;
  iss>>x;
  return x;

}

int READ_INT(int low ,int high)
{
 cout<<"choose from range 1 :"<<high<<endl;
 int choice;
 cin>>choice;

 if(choice>=1&&choice<=high)
  return choice;

  cout<<"invalid try again \n";
  READ_INT(low,high);

}
int SHOW_READ_MENU(vector<string>lines)
{  cout<<"\nMENU:\n";

for(int i=0;i<(int)lines.size();i++)
cout<<"  "<<i+1<<" : "<<lines[i]<<endl;

return READ_INT(1,lines.size());

}

struct USER
{
int user_id,allow_anonymous;
string user_name,password,name,email;
vector<int>questionsid_from_me;
map<int,vector<int>>questionsid_to_me;
USER()
{
user_id=allow_anonymous=-1;
}
USER(string str)
{
vector<string>lines=SPLIT(str);
user_id=To_INT(lines[0]);
user_name=lines[1];
password=lines[2];
name=lines[3];
allow_anonymous=To_INT(lines[4]);
email=lines[5];


}

	string ToString() {
		ostringstream oss;
		oss << user_id << "," << user_name << "," << password << "," << name << "," << allow_anonymous<< email<<endl ;

		return oss.str();
	}

	void Print() {
		cout << "User " << user_id << ", " << user_name << " " << password << ", " << name << ", " << email << "\n";
	}


};

struct QUESTION
{
int question_id,question_parent,from,to,is_anonymous;
string answer,question;

QUESTION()
{
is_anonymous=1;
question_parent=question_id=from=to=-1;

}
QUESTION(string str)
{
vector<string>lines=SPLIT(str);
question_id=To_INT(lines[0]);
question_parent=To_INT(lines[1]);
from=To_INT(lines[2]);
to=To_INT(lines[3]);
is_anonymous=To_INT(lines[4]);
question=lines[5];
answer=lines[6];


}


string ToString() {
ostringstream oss;
oss << question_id << "," << question_parent << "," << from << "," << to << "," << is_anonymous << "," << question << "," << answer;

return oss.str();
	}

	void print_to_question() {
		string prefix = "";

		if (question_parent != -1)
			prefix = "\tThread: ";

		cout << prefix << "Question Id (" << question_id << ")";
		if (!is_anonymous)
			cout << " from user id(" << from << ")";
		cout << "\t Question: " << question << "\n";

		if (answer!= "")
			cout << prefix << "\tAnswer: " << answer << "\n";
		cout << "\n";
	}

	void print_from_question() {
		cout << "Question Id (" << question_id << ")";
		if (!is_anonymous)
			cout << " !AQ";

		cout << " to user id(" << to << ")";
		cout << "\t Question: " << question;

		if (answer != "")
			cout << "\tAnswer: " << answer << "\n";
		else
			cout << "\tNOT Answered YET\n";
	}

	void Print_Feed_Question() {
		if (question_parent != -1)
			cout << "Thread Parent Question ID (" << question_parent << ") ";

		cout << "Question Id (" << question_id << ")";
		if (!is_anonymous)
			cout << " from user id(" << from << ")";

		cout << " To user id(" << to << ")";

		cout << "\t Question: " << question << "\n";
		if (answer != "")
			cout << "\tAnswer: " << answer <<"\n";
	}





};

struct Question_Manger
{
map<int,vector<int>>questionid_questionsthread;
map<int,QUESTION>questionid_question;
int last_id;

Question_Manger()
{
last_id=0;
}
void LoadDatabase()
{ last_id=0;
 questionid_question.clear();
 questionid_questionsthread.clear();
 vector<string>lines=READ_FILE("questions.txt");
 for(string &line:lines)
 {
 QUESTION question(line);
 last_id=max(last_id,question.question_id);
 if(question.question_parent==-1)
   questionid_questionsthread[question.question_id].push_back(question.question_id);
   else
    questionid_questionsthread[question.question_parent].push_back(question.question_id);
    questionid_question[question.question_id]=question;


 }
 }

 void FillUserDataBase(USER user)
 {
 user.questionsid_from_me.clear();
 user.questionsid_to_me.clear();
 for(auto p:questionid_questionsthread){
 for(auto q:p.second)
 {
 QUESTION &question=questionid_question[q];
 if(question.to==user.user_id)
 {
   if(question.question_parent==-1)
     user.questionsid_to_me[question.question_id].push_back(question.question_id);
     else
     user.questionsid_to_me[question.question_parent].push_back(question.question_id);

 }
 else if(question.from==user.user_id)
 user.questionsid_from_me.push_back(question.question_id);


 }
 }

 }


void print_to_user(USER user)
{

 if(user.questionsid_to_me.size()==0)
 cout<<"NO questinos \n";
 for(auto p:user.questionsid_to_me)
 {
 for(auto i:p.second)
 {
 QUESTION question=questionid_question[i];
 question.print_to_question();

 }
 cout<<endl;
 }


}

void print_from_user(USER user)
{

 if(user.questionsid_from_me.size()==0)
 cout<<"NO questinos \n";
 for(auto p:user.questionsid_from_me)
 {

 QUESTION question=questionid_question[p];
 question.print_from_question();

 }
 cout<<endl;
}

int Read_Question_id(USER user)
{
cout<<"enter question id or -1 to cancel\n";
int x;
cin>>x;
if(x==-1)return x;
if(!questionid_question.count(x)||questionid_question[x].to!=user.user_id)
{cout<<"invalid ...try again\n"; Read_Question_id(user);}

return x;


}
void answer_question(USER user)
{
int choice=Read_Question_id(user);
if(choice ==-1)return ;
QUESTION &question=questionid_question[choice];
question.print_to_question();
if(question.answer!="")
cout<<"warning alread answered .. answer will be updated\n";
cout<<"enter answer \n";
getline(cin,question.answer);
getline(cin,question.answer);


}

void delete_question(USER user)
{
int choice=Read_Question_id(user);
if(choice==-1)return ;
vector<int>to_remove;
if(questionid_questionsthread.count(choice))
{
to_remove=questionid_questionsthread[choice];
questionid_questionsthread.erase(choice);
}
else
{
to_remove.push_back(choice);
for(auto p:questionid_questionsthread)
{
vector<int>&v=p.second;
for(int i=0;i<v.size();i++)
{
if(v[i]==choice)
{
v.erase(v.begin()+i);
break;

}
}


}
}

for(int id:to_remove)
questionid_question.erase(id);


}

int Read_Thread_Question(USER user)
{ int choice;
cout<<"for thread question enter question id or -1 to new question\n ";
cin>>choice;
if(choice ==-1)return -1;
if(!questionid_question.count(choice))
{
 cout<<"not found... try again \n";
 Read_Thread_Question(user);
}
return choice;

}
void Ask_Question(USER &user,pair<int,int>p)
{
QUESTION question;
if(p.first==-1)return;
if(!p.second)
{cout<<"user does not allow anonymous questions\n";
question.is_anonymous=0;
}
else
{
cout<<"is anonymous question 0 or 1 : \n";
cin>>question.is_anonymous;
}

question.question_parent=Read_Thread_Question(user);
cout<<"enter question :\n";
getline(cin,question.question);
getline(cin,question.question);
question.question_id= ++last_id;

questionid_question[question.question_id]=question;

if(question.question_parent==-1)
questionid_questionsthread[question.question_id].push_back(question.question_id);
else
questionid_questionsthread[question.question_parent].push_back(question.question_id);

}
void ListFeed() {
		for (auto &pair : questionid_question) {
			QUESTION &question = pair.second;

			if (question.answer == "")
				continue;

			question.Print_Feed_Question();
		}
	}

	void UpdateDatabase()
	{
	vector<string>lines;
	for(auto i:questionid_question)
	lines.push_back(i.second.ToString());

	WRITE_FILE("questions.txt",lines,false);


	}






};

struct User_Manger
{
map<string,USER>username_to_user;
int last_id;
USER current_user;

User_Manger()
{ last_id=0;
}

void LoadDataBase()
{
last_id=0;
username_to_user.clear();
vector<string>lines=READ_FILE("users.txt");
for(string line:lines)
{
USER user(line);
last_id=max(last_id,user.user_id);
username_to_user[user.user_name]=user;
}

}

void Access_system()
{
int choice=SHOW_READ_MENU({"login","signup"});
if(choice==1)
  login();
  else
  signup();

}

void login()
{
LoadDataBase();
while(true)
{
cout << "Enter user name & password: ";
cin>>current_user.user_name>>current_user.password;

if(!username_to_user.count(current_user.user_name))
{
cout << "\nInvalid user name or password. Try again\n\n";
				continue;
}

if(username_to_user[current_user.user_name].password!=current_user.password)
{
cout << "\nInvalid user name or password. Try again\n\n";
				continue;
}

current_user=username_to_user[current_user.user_name];
break;

}

}

void signup()
{
while (true) {
			cout << "Enter user name. (No spaces): ";
			cin >> current_user.user_name;

			if (username_to_user.count(current_user.user_name))
				cout << "Already used. Try again\n";
			else
				break;
		}

		cout << "Enter password: ";
		cin >> current_user.password;

		cout << "Enter name: ";
		cin >> current_user.name;

		cout << "Enter email: ";
		cin >> current_user.email;

		cout << "Allow anonymous questions? (0 or 1): ";
		cin >> current_user.allow_anonymous;


		current_user.user_id = ++last_id;
		username_to_user[current_user.user_name] = current_user;

		UpdateDatabase(current_user);

}

pair<int,int> Read_User_Id()
{
cout<<"enter user id or -1 to cancel \n";
int x;
cin>>x;
if(x==-1)
return make_pair(-1,-1);

for(auto p: username_to_user)
{
if(p.second.user_id==x)
return make_pair(x,p.second.allow_anonymous);
}

cout<<"not found \n";
Read_User_Id();

}

	void ListUsersNamesIds() {
		for (auto &p : username_to_user)
			cout << "ID: " <<p.second.user_id << "\t\tName: " << p.second.name << "\n";
	}

void UpdateDatabase(USER user)
{
string str=user.ToString();
vector<string>s(1,str);
WRITE_FILE("users.txt",s);

}

};


struct SERVICE
{
Question_Manger q;
User_Manger u;

void LoadDatabase(bool b=false)
{

q.LoadDatabase();
u.LoadDataBase();
if(b)
{
q.FillUserDataBase(u.current_user);

}

}

	void run() {
		LoadDatabase();
		u.Access_system();
		q.FillUserDataBase(u.current_user);

		vector<string> menu;
		menu.push_back("Print Questions To Me");
		menu.push_back("Print Questions From Me");
		menu.push_back("Answer Question");
		menu.push_back("Delete Question");
		menu.push_back("Ask Question");
		menu.push_back("List System Users");
		menu.push_back("Feed");
		menu.push_back("Logout");

		while (true) {
			int choice = SHOW_READ_MENU(menu);
			LoadDatabase(true);

			if (choice == 1)
				q.print_to_user(u.current_user);

			else if (choice == 2)
				q.print_from_user(u.current_user);
			else if (choice == 3) {
				q.answer_question(u.current_user);
				q.UpdateDatabase();
			} else if (choice == 4) {
				q.delete_question(u.current_user);
				// Let's build again (just easier, but slow)
				q.FillUserDataBase(u.current_user);
				q.UpdateDatabase();
			} else if (choice == 5) {
				pair<int, int> to_user_pair = u.Read_User_Id();
				if (to_user_pair.first != -1) {
					q.Ask_Question(u.current_user, to_user_pair);
					q.UpdateDatabase();
				}
			} else if (choice == 6)
				u.ListUsersNamesIds();
			else if (choice == 7)
				q.ListFeed();
			else
				break;
		}
		run();
	}

};
int main()
{

SERVICE ASK;
ASK.run();
    return 0;
}
