#ifndef STUDENT_H_
#define STUDENT_H_
#include <string>
#include <vector>

class Class;

enum Degree {
    undergraduate,
    graduate
};

class Student {
private:
    std::string name;
    std::string year;
    Degree degree;

protected:
    std::vector<Class*> attendedClass;


public:
    const std::string id;
    std::string toString() const; //used for getting student information

    //construction
    Student(const std::string& id, const std::string& name, const std::string& year, Degree degree)
        : id(id), name(name), year(year), degree(degree) {}
    //virtual distruction
    virtual ~Student() {}

    //use pure virtual function
    virtual double getGpa() const=0;
    virtual double getAvgScore() const=0;

    void attendClass(Class  * atcl);  //used to add all the subjects the stu attend


};

//Override the virtual function
class Graduate : public Student {

public:
    Graduate(const std::string& id, const std::string& name, const std::string& year)
    : Student(id, name, year, graduate) {}
    double getGpa() const ; // override the Implements GPA calculation for graduates
    double getAvgScore() const ;


};



class Undergraduate : public Student {
public:
    Undergraduate(const std::string& id, const std::string& name, const std::string& year)
        : Student(id, name, year, undergraduate) {}
    double getGpa() const ;
    double getAvgScore() const ;


};


//serve as a wrapper of data and function,in order to realize Separation of Concerns
class StudentWrapper {
private:
    //Contains data of a reference to Student & score
    const Student & student;
    double score = -1.0;
public:
    const std::string id;

    // TODO: fix error
    // const member variables should be initialized in the initialization list
    StudentWrapper(const Student &student,const std::string &id)
    : student(student),id(id) { }

    void setScore(double score)
    {
        //Use "throw-catch" to handle exceptions by throwing them upwards
        if (score < 0 || score > 100)
            throw "Wrong score!";
        this->score = score;
    }

    double getScore() const
    {
        return this->score;
    }

    std::string toString() const
    {
        return student.toString();
    }
};

#endif // STUDENT_H_
