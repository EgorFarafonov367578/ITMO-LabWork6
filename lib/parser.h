#pragma once

#include <iostream>
#include <istream>
#include <filesystem>
#include <fstream>
#include <map>
#include <stack>
#include <string>
#include <vector>

namespace omfl {

enum ClassOfInputStream { StringStream, FileStream };

class InputStream {
 private:
  ClassOfInputStream class_of_input_stream_;
  std::ifstream file_stream_;
  std::istringstream string_stream_;
  std::stack<std::string> stack_of_future_words_;
 public:
  bool IsEmptuty() {
    return (stack_of_future_words_.empty());
  }
  bool operator>>(std::string& string) {
    if (!stack_of_future_words_.empty()) {
      string = stack_of_future_words_.top();
      stack_of_future_words_.pop();
      if (string == "") {
        return ((*this) >> string);
      }
      if (string == "#") {
        std::cout << "hi" << std::endl;
        if (!stack_of_future_words_.empty())stack_of_future_words_ = std::stack<std::string> ();
        else {
          std::string place;
          if (class_of_input_stream_ == ClassOfInputStream::FileStream) {
            std::getline(file_stream_,place);
          }
          else {
            std::getline(string_stream_,place);
          }
        }

        return (*this) >> string;
      }

      return true;
    }
    if (class_of_input_stream_ == ClassOfInputStream::FileStream) {
      if (file_stream_.eof()) {
        return false;
      }
      file_stream_ >> string;
      if (string == "") {
        return false;
      }
      if ((string[0] == '[') && (string[string.size()-1] == ']')) {
        std::string ans;
        for (int i = 1; i < string.size()-1; i++){
          ans.push_back(string[i]);
        }
        (*this) << "]" << ans << "[";

        return ((*this) >> string);
      }
      if (string == "#") {
        std::getline(file_stream_,string);

        return ((*this) >> string);
      }
      std::stack <std::string> stack;
      std::string future_word;
      for (char x: string) {
        if (x == '=') {
          if (future_word!= "") {stack.push(future_word);}
          future_word = "";
          stack.push("=");
        }
        else {
          future_word.push_back(x);
        }
      }
      while (!stack.empty()) {
        stack_of_future_words_.push(stack.top());
        stack.pop();
      }

      return ((*this) >> string);
    } else {
      if (string_stream_.eof()) {std::cout << "Final point2" << std::endl;return false;}
      string_stream_ >> string;
      std::cout << string << " here1" <<std::endl;
      if (string == "") {std::cout << "Final point1" << std::endl;return false;}
      if ((string[0] == '[') && (string[string.size()-1] == ']')) {
        std::string ans;
        for (int i = 1; i < string.size()-1; i++){
          ans.push_back(string[i]);
        }
        (*this) << "]" << ans << "[";

        return ((*this) >> string);
      }
      std::cout << string << " here2" <<std::endl;
      if (string == "#") {
        std::getline(string_stream_,string);
        std::cout << "WTF"<< string << "|" << std::endl;
        std::cout << "CP1" << std::endl;

        return ((*this) >> string);
      }
      std::stack <std::string> stack;
      std::string future_word;
      for (char x: string) {
        if (x == '=') {
          if (future_word!= "") {stack.push(future_word);}
          future_word = "";
          stack.push("=");
        }
        else {
          future_word.push_back(x);
        }
      }
      if (future_word!= "") {stack.push(future_word);}
      std::cout << string << std::endl;
      while (!stack.empty()) {
        stack_of_future_words_.push(stack.top());
        stack.pop();
        std::cout << "+1" << std::endl;
      }

      return ((*this) >> string);
    }
  }
  InputStream& operator<<(const std::string& string) {
    stack_of_future_words_.push(string);

    return (*this);
  }
  InputStream(const std::string& string) {
    class_of_input_stream_ = ClassOfInputStream::StringStream;
    string_stream_ = std::istringstream(string);
  }
  InputStream(const std::filesystem::path& path) {
    class_of_input_stream_ = ClassOfInputStream::FileStream;
    file_stream_ = std::ifstream(path);
  }
  bool PrepearForArgument();
};
//class __NameMe__;
//__NameMe__ parse(const std::filesystem::path& path);
//__NameMe__ parse(const std::string& str);

enum Class_of_Element_of_OMFL { Bool, Int, Float, String, Array };

enum Class_of_Selection { Element, Section, Error };

class OMFL {
  friend bool ParseSection(InputStream& input_stream, OMFL& section, const std::string& section_name);
  friend void PrintSection(const OMFL& section);
  friend bool ParseElement(omfl::InputStream& input_stream, omfl::OMFL& element);
  friend OMFL parse(omfl::InputStream& input_stream);
 private:
  Class_of_Selection class_of_OMFL_object_;
  std::map<std::string, OMFL> sub_sections_;
  Class_of_Element_of_OMFL class_of_OMFL_element_;
  bool bool_stored_value_;
  long long int int_stored_value_;
  double float_stored_value_;
  std::string string_stored_value_;
  std::vector<OMFL> array_;
 public:
  OMFL(std::string): class_of_OMFL_object_(Class_of_Selection::Error){}
  OMFL(){}

  bool valid() const;

  bool IsBool() const;
  bool IsInt() const;
  bool IsFloat() const;
  bool IsString() const;
  bool IsArray() const;

  bool AsBool() const;
  long long int AsInt() const;
  double AsFloat() const;
  const std::string& AsString() const;

  bool AsBoolOrDefault(bool default_value) const;
  long long int AsIntOrDefault(long long int default_value) const;
  double AsFloatOrDefault(double default_value) const;
  const std::string& AsStringOrDefault(const std::string& default_value) const;

  const OMFL& Get(const std::string& sub_section_name) const;
  const OMFL& operator[](uint32_t number_of_element_in_array) const;
};

OMFL parse(const std::filesystem::path& path);
OMFL parse(const std::string& str);
OMFL parse(omfl::InputStream& input_stream);
void PrintSection(const OMFL& section);


}// namespace