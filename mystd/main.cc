#include <iostream>
#include "vector.h"
#include <stdlib.h> 
using namespace mystd;
class A{
    public:
        char *str;

        explicit A(int a){
            if (a != 0){
                str = (char *)malloc(a+1);
                for(int i = 0; i < a; i ++)
                    str[i] = rand()%26+'a';
                str[a]='\0';
            }
            else
                str = NULL;
            std::cout << str << "?"<< (void *) str << std::endl;
        }

        A(const A& a){
            int length = a.size()+1;
            str = (char *)malloc(length);
            strcpy(str, a.str);
            str[length-1] = '\0';
            std::cout << str << "?"<< (void *) str << std::endl;
        }

        const int size() const {
            return strlen(str);
        }

        ~A(){
            std::cout << str << ":"<< (void *) str << std::endl;
            if(str)
                free(str);
            str = 0;
        }

};

int main(int argc, char **argv){
    vector<int> v(4, 1);
    vector<A> a;
    A temp(10);
    a.push_back(temp);
    a.push_back(temp);
    a.push_back(temp);
    a.push_back(temp);
    a.push_back(temp);
    a.push_back(temp);
    return 0;
}
