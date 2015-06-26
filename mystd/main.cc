#include <iostream>
#include "vector.h"
#include <stdlib.h> 
#include <vector>
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

        A& operator=(A & other){
            if (this != &other){
                std::cout << "move"<< (void *) str <<  " = " << (void *)other.str << std::endl;
                free(str);
                str = other.str;
                other.str = NULL;
            }
            return *this;
        }

        const int size() const {
            return strlen(str);
        }

        ~A(){
            std::cout << ":"<< (void *) str << std::endl;
            if(str)
                free(str);
            str = NULL;
        }

};

int main(int argc, char **argv){
    /*
    std::vector<A> a(10, A(5));
    std::cout << a.size() << std::endl;
    a.erase(a.begin());
    std::cout << a.size() << std::endl;
    */
    vector<A> a;
    for(int i = 1; i < 11; i++){
        A temp(i);
        a.push_back(temp);
    }
    std::cout << a.size() << std::endl;
    a.erase(a.begin());
    std::cout << a.size() << std::endl;
    return 0;   
}
