#include <iostream>
#include <string>
#include <queue>
using namespace std;

char LowerCase(char lower) {
    if (lower >= 'A' && lower <= 'Z') {
        return lower + ('a' - 'A');
    }
    return lower;
}

int main() {
    string input;
    cout << "enter a paragraph\n";
    getline(cin, input); 
    cout<<"\n";

    // for aplhabets
    queue<string> letter_queues[26];

    string currentword;
    for (int i = 0; i <= input.length(); ++i) {
      
        if (i == input.length() || input[i] == ' ') {

            if (!currentword.empty()) {  
            
                char firstchar = LowerCase(currentword[0]); //see first letter
                int index = firstchar - 'a';  
            
                if (index >= 0 && index < 26) {  
                    letter_queues[index].push(currentword);  // push to specific queue
                }
            
                currentword.clear();
            }

        } 
        else {
            currentword += input[i]; 
        }
    }

    string sorted_paragraph;

    for (int i = 0; i < 26; ++i) {
        
        while (!letter_queues[i].empty()) {
        
            sorted_paragraph += letter_queues[i].front() + " ";  
            letter_queues[i].pop();  
        
        }
    }

    cout << "the sorted paragraph is : \n" << sorted_paragraph ;
    cout<< endl;

    return 0;
}