#include<iostream>
#include<vector>
using namespace std;
int main(){
    string input;
    char ch;
    vector<string> obj;
    while(ch!='\n'){
        cin>>input;
        cin.get(ch); //捕获空白符，如‘ ’，‘\n’，‘\t’
        obj.push_back(input);
        if(ch!='\n'){
            obj.push_back(" ");
        }
    }
    for (int i = 0; i < obj.size(); ++i) {
        cout<<obj[i]<<" ";
    }
    cout<<endl;
    return 0;
}
