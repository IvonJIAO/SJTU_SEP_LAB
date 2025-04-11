#include "Class.h"
#include <string>
#include <vector>
#include "Student.h"
#include <sstream>
#include <fstream>
#include <iostream>

std::string Class::toString() const
{
    std::ostringstream oss;
    oss << "Class Information:"
        << "\n\tname: " << name
        << "\n\tpoint: " << point
        << std::endl;
    return oss.str();
}


//add student data
void Class::addStudent(const Student& st)
{
    StudentWrapper sw(st,st.id);
    students.push_back(sw);
}

//return the reference,avoid useless copy
StudentWrapper& Class::getStudentWrapper(const std::string& studentId)
{
    for (auto & student : students) {
        if (student.id == studentId)
            return student;  //it is an iterator type(similar to a pointer),so we should return *it
    }
    throw "No Match Student!";
}


double Class::getHighestScore() {

    double maxScore = 0; // initialize as 0
    bool hasValidScore = false;

    for (const auto& sw : students) {
        if (sw.getScore() >= 0)
            {
            if (sw.getScore() >= maxScore) {
                maxScore = sw.getScore();
                hasValidScore = true;
            }
        }
    }

    if (!hasValidScore) {
        throw "No Valid Score";
    }

    return maxScore;
}

double Class::getLowestScore() {

    double minScore = 100;
    bool hasValidScore = false;

    for (const auto& sw : students) {
        if (sw.getScore() >= 0)
            {
            if (sw.getScore() <= minScore) {
                minScore = sw.getScore();
                hasValidScore = true;
            }
        }
    }

    if (!hasValidScore)
    {
        throw "No Valid Score";
    }

    return minScore;
}


double Class::getAvgScore() {
    double sum = 0;
    double count = 0;
    bool hasValidScore = false;
    for (auto & student : students)
    {
        if (student.getScore() !=-1)
        {
            count++;
            sum += student.getScore();
            hasValidScore = true;
        }
    }

    if (!hasValidScore) {
        throw "No Valid Score";
    }
    return sum/count;
}


 void Class::saveScore(const std::string& filename)
{
     std::ofstream outFile(filename,std::ios::app); //The default mode is ios_base::out
      if (!outFile.is_open())
      {
          std::cerr << "Failed to open file: " << filename << std::endl;
      }

    outFile<<name<<std::endl;
     for (auto& sw : students)
     {
          double score = sw.getScore();
          if (score != -1)
          {
              outFile << sw.id<<"," << score << std::endl; // Saves scores to a file
          }
     }
      outFile.close();
 }
// void Class::saveScore(const std::string& filename) {
//     std::ofstream outFile(filename, std::ios::app);  // 追加模式
//
//     if (!outFile.is_open()) {
//         std::cerr << "无法打开文件进行保存!" << std::endl;
//         return;
//     }
//
//     // 写入课程名称
//     outFile << name << std::endl;
//
//     // 遍历所有学生并保存成绩
//     for (auto& sw : students) {
//         double score = sw.getScore();  // 获取学生成绩
//         if (score != -1) {  // 确保成绩有效
//             outFile << sw.student->id << "," << score << " ";  // 将学生ID和成绩写入文件，空格分隔
//         }
//     }
//
//     outFile << std::endl;  // 每个课程后添加换行
//     outFile.close();
// }
