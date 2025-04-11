#include "Student.h"
#include <string>
#include <sstream>
#include "Class.h"

std::string Student::toString() const {

    std::ostringstream oss;
    oss << "Student Information:"
            << "\n\tid: " << id
            << "\n\tname: " << name
            << "\n\tenrollment year: " << year
            << "\n\tdegree: " << (degree == graduate ? "graduate" : "undergraduate")
            << std::endl;
    return oss.str();
}

void Student::attendClass(Class *atcl)
{
    attendedClass.push_back(atcl);
}


double Undergraduate::getAvgScore() const {
    double sum = 0.0;
    int totalPoints = 0;
    for (auto &cl: attendedClass) {
        double score = cl->getStudentWrapper(id).getScore();
        if (score != -1) {
            sum += score * cl->point; // Weighted sum for undergraduates
            totalPoints += cl->point;
        }
    }
    if (totalPoints == 0) return sum;
    return sum / static_cast<double>(totalPoints); // Weighted average for undergraduates
}


double Graduate::getAvgScore() const {
    double sum = 0.0;
    int totalPoints = 0;
    for (auto &cl: attendedClass) {
        double score = cl->getStudentWrapper(id).getScore();

        //how to call Standard Error Stream?
        //no need to,if no course are attended ,we assume it 0.0
        if (score != -1.0)
        {
            sum += score * cl->point; // Weighted sum for undergraduates
            totalPoints += cl->point;
        }
    }
    if (totalPoints == 0) return 0.0;
    return sum / static_cast<double>(totalPoints); // Weighted average for undergraduates
}

double Undergraduate::getGpa() const {
    double sum = 0.0;
    int totalPoints = 0;
    for (auto &cl: attendedClass){
        double score = cl->getStudentWrapper(id).getScore();
        if (score != -1.0) {
            sum += score/20.0 * cl->point;
            totalPoints += cl->point;
        }
    }
    if (totalPoints == 0) return sum;    //when there is no attended courses,we output 0.0
    return sum / static_cast<double>(totalPoints);
}

double Graduate::getGpa() const {
    int totalPoints = 0;
    double sum = 0.0;
    for (auto &cl: attendedClass)
    {
        double score = cl->getStudentWrapper(id).getScore();
        if (score != -1.0) {
            switch (static_cast<int>(score / 10)) {
                case 10:
                case 9: sum += cl->point * 4.0;
                totalPoints += cl->point;
                break;
                case 8: sum += cl->point * 3.5;
                totalPoints += cl->point;
                break;
                case 7: sum += cl->point * 3.0;
                totalPoints += cl->point;
                break;
                case 6: sum += cl->point * 2.5;
                totalPoints += cl->point;
                break;
                default:
                    sum += cl->point * 2.0;
                totalPoints += cl->point;
                break;
            }
        }
    }
    if (totalPoints == 0) return 0.0;
    return sum / static_cast<double>(totalPoints);
}
