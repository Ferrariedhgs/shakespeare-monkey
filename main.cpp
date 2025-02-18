#include <iostream>
#include <string>
#include <fstream>
#include <csignal>
#include <cstdlib>
#include <windows.h>
#include <vector>
#include <time.h>


//ascii 'a'=97


std::ofstream sentences_file("sentences.txt"); //write correct sentences to file; a sentence is made from words in the dictionary
//if a word isn't from the dictionary the sentence ends
std::ofstream words_log("words_log.txt"); //write all generated words to a file; not recommended if you leave this running for more than a few minutes


int main()
{
    auto main_exit = [] () { sentences_file.close(); words_log.close(); std::cout<<"exited safely";};
    atexit(main_exit); //makes sure to safely close the files when you exit

    //int current_line_nr=0;
    std::string current_line="";
    int line_number=0;
    std::ifstream f("words_alpha.txt"); //used dwyl's english-words words_alpha.txt

    if(!f.is_open()) std::cout<<"error opening input file";


    while(!f.eof()) //get no. of lines
    {
        f>>current_line;
        line_number++;
    }
    //f.seekg(0,ios::beg); //return to first line
    f.close();
    f.open("words_alpha.txt");

    const int word_list_length=line_number;
    //std::string word_list[word_list_length]; //c++ doesn't like when the array doesn't have a fixed size at compilation time
    std::vector<std::string> word_list(word_list_length);


    for(int i=0;i<word_list_length;i++) //read each word and put it in the vector
    {
        getline(f,current_line);
        word_list[i]=current_line;
    }
	std::cout<<"loaded "<<word_list_length<<" words\n";
    f.close();

    int seed=time(NULL);
    std::cout<<"using seed "<<seed<<std::endl;
    srand(seed);
    //bool sentence_streak=0;
    int sentence_length=0;
    std::string current_word="";
    std::string current_sentence="";
    char current_char;
    std::string record_sentence="";
    int sentence_length_record=0;

    while(1==1)
    {
        current_char=(char)(96+rand()%(122-96+1)); //27 letters+space
        if(current_char==96) //is space=the word is completed
        {
            //words_log<<" "<<current_word;  //file gets big fast
            bool found=0;
            for(int i=0;i<word_list_length;i++) //try to find the word in the dictionary
            {
                if(word_list[i]==current_word)
                {
                    found=1;
                    break;
                }
            }
            if(found) //the word is in the dictionary; add to the sentence
            {
                //sentence_streak=1;
                sentence_length++;
                current_sentence+=" ";
                current_sentence+=current_word;
            }
            else //the word isn't in the dictionary; discard the word and write the sentence
            {
                //sentence_streak=0;
                if(sentence_length>1)
                {
                    sentences_file<<current_sentence<<"\n";
                    std::cout<<current_sentence;
                }
                if(sentence_length>sentence_length_record)
                {
                    sentence_length_record=sentence_length;
                    record_sentence=current_sentence;
                    sentences_file<<"NEW SENTENCE RECORD: "<<sentence_length_record<<"\t"<<record_sentence<<"\n";
                }

                current_sentence="";
                sentence_length=0;
            }
            current_word="";
        }
        else //is letter; add to the word
        {
            current_word+=current_char;
        }

        //Sleep(100);  //if you want it to go slower
    }


    sentences_file.close();
    words_log.close();
    return 0;
}
