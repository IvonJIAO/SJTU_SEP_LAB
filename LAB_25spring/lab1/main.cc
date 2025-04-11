#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "Class.h"
#include "Student.h"


using namespace std;

//Object-oriented programming
class AppX
{
private:
    vector<Student *> studentVec; //data type is a pointer to Student type
    vector<Class *> classVec;

    void loadFiles();
    void inputScore();
    void printScoreStats();
    void printGrade();
    void saveScore();

public:
    AppX();
    ~AppX();
    int run();
};

AppX::~AppX()
{
    // You can use the traditional loop, which is more clear.
    for (vector<Class *>::iterator it = classVec.begin();
         it != classVec.end();
         ++it)
    {
        if (*it) delete (*it);
    }

    // You can use modern and simpler loops only if you know what it is doing.
    for (const auto &s: studentVec)
    {
        if (s) delete (s);
    }
}


AppX::AppX()
{
    loadFiles();
}

void AppX::loadFiles()
{
    string line;
    size_t pos1, pos2;  //size_t is an unsigned integer type used to represent size or index.
    vector<string> bufv;
    Student *st = nullptr;
    string clsname;
    int point;
    Class *cl = nullptr;

    // Open a file as a input stream.
    ifstream stfile("./Students.txt");

    while (getline(stfile, line))
    {
        if (line.empty()) // It's an empty line.
            continue;
        if (line[0] == '#') // It's a comment line.
            continue;

        // The bufv vector stores each column in the line.
        bufv.clear();
        // Split the line into columns.
        //   pos1: beginning of the column.
        //   pos2: end of the column.
        pos1 = 0;
        while (true) {
            //find ";"from pos1 and return the position of ";"
            pos2 = line.find(';', pos1 + 1);

            //"string::npos"is a static const in class string ,meaning for not finging the character
            //or the rear of the string
            if (pos2 == string::npos)
            { // No more columns in the current line
                // Save the last column (pos1 ~ eol).

                // "sbustr"is used to subtract the wanted part of the string from the whole body
                // the first parameter shows the starting position,
                // while the second parameter shows the length
                bufv.push_back(line.substr(pos1, string::npos));
                break;
            }

            else
            {
                // Save the column (pos1 ~ pos2).
                bufv.push_back(line.substr(pos1, pos2 - pos1));
            }
            pos1 = pos2 + 1;
        }

        //distribute the corresponding parts of the data
        if (bufv[3] == "U")
            st = new Undergraduate(bufv[0], bufv[1], bufv[2]);
        else
            st = new Graduate(bufv[0], bufv[1], bufv[2]);


        studentVec.push_back(st);
    }
    stfile.close();

    // Hint: how is student information read?
    //similar to read student information we'll accomplish it later

    std::ifstream clfile("./Classes.txt");
    std::string className;
    while (std::getline(clfile, line))
    {
        //jump the empty and comment line
        if (line.empty() || line[0] == '#') continue;

        if (line.find("Class:") == 0)
        {
            className = line.substr(6); // Extracts the class name
        }

        else if (line.find("Points:") == 0)
        {
            point = std::stoi(line.substr(7)); // Extracts the class points
            cl= new Class(className, point);
            classVec.push_back(cl); // Creates a class
        }

        else
        {
            for (auto& stu : studentVec)
            {
                if (stu->id == line)
                {
                    classVec.back()->addStudent(*stu);
                    stu->attendClass(classVec.back());// Adds a student to the class
                    break;
                }
            }
        }
    }
    clfile.close();
}


//when inputing scores,"q"means start with a new class;
//a double q means break the function
void AppX::inputScore()
{
    while (true)
    {
        std::string className;
        std::cin >> className;
        if (className == "q")
        {
            return;; // Restart for a new class input
        }

        Class* cl = nullptr;
        for (auto& c : classVec)
        {
            if (c->name == className)
            {
                cl = c; // Find the class
                break;
            }
        }
        if (!cl)
        {
            std::cerr << "No Match Class!" << std::endl;
            continue; // Restart for a new class input
        }
        while (true)
        {
            std::string stuinfo;
            std::cin >> stuinfo;
            if (stuinfo == "q")
            {
                break; // Exit student input loop and restart for a new class
            }
            // Find the position of the comma
            size_t commaPos = stuinfo.find(',');
            if (commaPos == std::string::npos)
            {
                std::cerr << "Invalid input format!" << std::endl;
                continue; // Skip to the next iteration
            }
            // Extract student ID and score
            std::string studentId = stuinfo.substr(0, commaPos);
            std::string scoreStr = stuinfo.substr(commaPos + 1);
            // Convert score to integer
            int score = 0;
            try {
                score = std::stoi(scoreStr);
            } catch (...) {
                std::cerr << "Invalid Format!" << std::endl;
                continue; // Skip to the next iteration
            }
            // Validate score range
            if (score < 0 || score > 100)
            {
                std::cerr << "Wrong Score! " << std::endl;
                continue; // Skip to the next iteration
            }
            // Find the student's wrapper and set the score
            try {
                StudentWrapper& sw = cl->getStudentWrapper(studentId);
                sw.setScore(score);
            } catch (...) {
                std::cerr << "No Match Student" << std::endl;
            }
        }
    }
}




void AppX::printScoreStats()
{
    string sbuf;
    Class *cl;//used to store the name of found subject
    double highest, lowest, avg;

    //control the input
    while (true) {
        //cout << "Please input the class name (or input q to quit): ";
        cin >> sbuf;
        if (sbuf == "q")
            break;

        cl = nullptr;
        for (vector<Class*>::iterator it = classVec.begin();
                it != classVec.end();
                ++ it) {
            if ((*it)->name == sbuf) {
                cl = *it;
                break;
            }
        }
        if (cl == nullptr) {
            cerr << "No Match Class!" << endl;
            continue;
        }

        try {
            highest = cl->getHighestScore();
            lowest = cl->getLowestScore();
            avg = cl->getAvgScore();

            //cout << cl->toString() << endl;
            cout << setiosflags(ios::fixed) << setprecision(2)<< "Highest,Lowest,Avg = " << highest << "," << lowest << "," << avg << endl;
        } catch (char const* e){
            cerr << e << endl;
        } catch (...) {
            cerr << "Unknown exception caught" << endl;
        }
    }
}



//get grade of a student


void AppX::printGrade() {
    std::string studentId;
    while (true) {
        //std::cout << "Enter student ID (or 'q' to quit): ";
        std::cin >> studentId;
        if (studentId == "q")
        {
            break; // 退出循环
        }

        try {
            Student* st = nullptr;
            for (auto& s : studentVec) {
                if (s->id == studentId) {
                    st = s; // 找到学生
                    break;
                }
            }
            if (!st)
            {
                throw "No Match Student!";
            }
            double gpa = st->getGpa(); // 获取学生的 GPA
            double avg = st->getAvgScore(); // 获取学生的平均分
            std::cout <<fixed<<setprecision(2)<< "GPA,AVG = " << gpa << "," << avg << std::endl;
        }
        catch (const char* e)
        {
            std::cerr << e << std::endl;
        }
    }
}





//To save grade in a new txt file
void AppX::saveScore()
{
    std::ofstream outFile("./scores.txt", std::ios::trunc);
    // 遍历所有课程并调用各课程的 saveScore 函数
    for (auto& cl : classVec)
    {
        cl->saveScore("./scores.txt");  // 将成绩保存到文件中
    }

}



int AppX::run()
{
    char cmd;
    while (true) {
        cout << "Command menu:\n"
            << "\ti: Input score\n"
            << "\ta: Compute score statistics of a class\n"
            << "\tg: Compute grade of a student\n"
            << "\tq: Quit\n"
            << "Please input the command: ";
        cin >> cmd;

        //Interact with the user for feature selection
        if (cmd == 'i') {
            inputScore();
        } else if (cmd == 'a') {
            printScoreStats();
        } else if (cmd == 'g') {
            printGrade();
        } else if (cmd == 'q') {
            saveScore();
            break;
        } else {
            cout << "Invalid Command!\n" << endl;
        }
    }
    return 0;
}

int main()
{
    //we call "Loadfile" when constructing app
    AppX app;
    return app.run();
}
