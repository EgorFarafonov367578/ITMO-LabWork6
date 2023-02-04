#include "parser.h"

#include <iostream>
#include <queue>

bool omfl::InputStream::PrepearForArgument() {
  std::stack<std::string> save_stack;
  while (!stack_of_future_words_.empty()) {
    save_stack.push(stack_of_future_words_.top());
    stack_of_future_words_.pop();
  }
  std::string input_string;
  std::stack<std::string> stack;
  if (class_of_input_stream_ == ClassOfInputStream::FileStream) {
    std::getline(file_stream_, input_string);
  } else {
    std::getline(string_stream_, input_string);
  }
  if (input_string == "") {
    return false;
  }
  std::string future_word;
  for (int i = 0; i < input_string.size(); i++) {
    if (input_string[i] == '"') {
      future_word.push_back(input_string[i]);
      i++;
      bool check = false;
      while (i != input_string.size()) {
        if (input_string[i] == '"') {
          future_word.push_back(input_string[i]);
          check = true;
          break;
        } else {
          future_word.push_back(input_string[i]);
        }
        i++;
      }
      if (!check) return false;
      stack.push(future_word);
      future_word = "";
      continue;
    }
    if (input_string[i] == '[') {
      if (future_word != "") stack.push(future_word);
      future_word = "";
      stack.push("[");
      continue;
    }
    if (input_string[i] == ']') {
      if (future_word != "") stack.push(future_word);
      future_word = "";
      stack.push("]");
      continue;
    }
    if (input_string[i] == ',') {
      if (future_word != "") stack.push(future_word);
      future_word = "";
      stack.push(",");
      continue;
    }
    if (input_string[i] == ' ') {
      if (future_word != "") stack.push(future_word);
      future_word = "";
      continue;
    }
    if (input_string[i] == '=') {
      if (future_word != "") stack.push(future_word);
      future_word = "";
      stack.push("=");
      continue;
    }
    future_word.push_back(input_string[i]);
  }
  if (future_word != "") stack.push(future_word);
  future_word = "";
  while (!stack.empty()) {
    stack_of_future_words_.push(stack.top());
    stack.pop();
  }
  while (!save_stack.empty()) {
    stack_of_future_words_.push(save_stack.top());
    save_stack.pop();
  }

  return true;
}

omfl::OMFL omfl::parse(const std::filesystem::path& path) {
  omfl::InputStream input_stream(path);

  return parse(input_stream);
}

omfl::OMFL omfl::parse(const std::string& str) {
  omfl::InputStream input_stream(str);

  return parse(input_stream);
}

bool CheckFloat(const std::string& s) {
  int c = 0;
  for (int i = 0; i < s.size(); i++) {
    if (s[i] == '.') {
      if ((i == 0) || (i == (s.size() - 1))) {
        std::cout << "Hi1";

        return false;
      }
      if ((s[i - 1] == '+') || (s[i - 1] == '-')) {
        std::cout << "Hi2";

        return false;
      }
      c++;
      continue;
    }
    if (('0' <= s[i]) && (s[i] <= '9')) {
      continue;
    }
    if ((i == 0) && ((s[i] == '-') || (s[i] == '+'))) {
      continue;
    }

    return false;
  }

  return (c == 1);
}

bool CheckInt(const std::string& s) {
  for (int i = 0; i < s.size(); i++) {
    if (('0' <= s[i]) && (s[i] <= '9')) {
      continue;
    }
    if ((i == 0) && ((s[i] == '-') || (s[i] == '+'))) {
      continue;
    }

    return false;
  }
  if (s == "+") {
    return false;
  }
  if (s == "-") {
    return false;
  }

  return ((s.size() > 0));
}

bool CheckString(const std::string& s) {
  if (s.size() < 2) {
    return false;
  }
  if ((s[0] != '"') || (s[s.size() - 1] != '"')) {
    return false;
  }

  return true;
}

bool CheckKey(const std::string s) {
  for (char c : s) {
    if (!((('a' <= c) && (c <= 'z')) || (('A' <= c) && (c <= 'Z')) || (('0' <= c) && (c <= '9')) || (c == '-')
        || (c == '_')))
      return false;
  }

  return true;
}

omfl::OMFL omfl::parse(omfl::InputStream& input_stream) {
  OMFL ans;
  ans.class_of_OMFL_object_ = omfl::Class_of_Selection::Section;
  if (ParseSection(input_stream, ans, "")) {
    return ans;
  }
  else {
    return OMFL("");
  }
}

bool omfl::ParseElement(omfl::InputStream& input_stream, omfl::OMFL& element) {
  std::string string;
  if (!(input_stream >> string)) {
    return false;
  }
  if (string == "[") {
    element.class_of_OMFL_element_ = Class_of_Element_of_OMFL::Array;
    while (true) {
      if (!(input_stream >> string)) {
        return false;
      }
      if (string == "]") break;
      if (string == ",") continue;
      else input_stream << string;
      OMFL elem_of_arr;
      elem_of_arr.class_of_OMFL_object_ = Class_of_Selection::Element;
      element.array_.push_back(elem_of_arr);
      if (!ParseElement(input_stream, element.array_[element.array_.size() - 1])) {
        return false;
      }
    }

    return true;
  }
  if (string == "true") {
    element.class_of_OMFL_element_ = Class_of_Element_of_OMFL::Bool;
    element.bool_stored_value_ = true;

    return true;
  }
  if (string == "false") {
    element.class_of_OMFL_element_ = Class_of_Element_of_OMFL::Bool;
    element.bool_stored_value_ = false;

    return true;
  }
  std::cout << "fl != " << string << "?" << std::endl;
  if (CheckFloat(string)) {
    element.class_of_OMFL_element_ = Class_of_Element_of_OMFL::Float;
    element.float_stored_value_ = std::stof(string);

    return true;
  }
  if (CheckInt(string)) {
    element.class_of_OMFL_element_ = Class_of_Element_of_OMFL::Int;
    element.int_stored_value_ = std::stoi(string);

    return true;
  }
  if (CheckString(string)) {
    element.class_of_OMFL_element_ = Class_of_Element_of_OMFL::String;
    for (int i = 1; i < string.size() - 1; i++) element.string_stored_value_.push_back(string[i]);

    return true;
  }

  return false;
}

bool omfl::ParseSection(omfl::InputStream& input_stream, omfl::OMFL& section, const std::string& section_name) {
  while (true) {
    std::string s;
    if (!(input_stream >> s)) {
      return true;
    }
    std::cout << "Begining of partion of section" << s << std::endl;
    if (s == "") {
      break;
    }
    if (s == "[") {
      if (!(input_stream >> s)) {
        return false;
      }
      std::string place;
      if (!(input_stream >> place)) {
        return false;
      }
      if (place != "]") {
        return false;
      }
      if (s.size() > section_name.size()) {
        bool check = true;
        for (int i = 0; i < section_name.size(); i++) {
          if (s[i] != section_name[i]) check = false;
        }
        if (check) {
          std::queue<std::string> queue;
          std::string future_section;
          for (int i = section_name.size(); i < s.size(); i++) {
            if (s[i] == '.') {
              queue.push(future_section);
              future_section = "";
            } else {
              future_section.push_back(s[i]);
            }
          }
          queue.push(future_section);
          OMFL* current_section = &section;
          while (!queue.empty()) {
            if (queue.front() == "") {
              return false;
            }
            //if (current_section->sub_sections_.find(queue.front()) == current_section->sub_sections_.end()) current_section->sub_sections_[queue.front()] = OMFL();
            current_section->sub_sections_[queue.front()].class_of_OMFL_object_ = omfl::Class_of_Selection::Section;
            current_section = &current_section->sub_sections_[queue.front()];
            queue.pop();
          }
          ParseSection(input_stream, *current_section, s + ".");
        } else {
          input_stream << "]" << s << (std::string("["));
          break;
        }
      } else {
        input_stream << "]" << s << (std::string("["));
        break;
      }
    } else {
      if (!CheckKey(s)) {
        return false;
      }
      if (!input_stream.PrepearForArgument()) {
        return false;
      }
      std::string s2;
      if (!(input_stream >> s2)) {
        return false;
      }
      std::cout << "Second part of section" << s2 << std::endl;
      if (s2 == "=") {
        if (input_stream.IsEmptuty()) {
          return false;
        }
        if (section.sub_sections_.find(s) != section.sub_sections_.end()) {
          return false;
        }
        section.sub_sections_[s] = OMFL();
        section.sub_sections_[s].class_of_OMFL_object_ = omfl::Class_of_Selection::Element;
        if (!ParseElement(input_stream, section.sub_sections_[s])) {
          return false;
        }
      } else {
        return false;
      }
    }
  }

  return true;
}

void omfl::PrintSection(const omfl::OMFL& section) {
  switch (section.class_of_OMFL_object_) {
    case (omfl::Element):
      switch (section.class_of_OMFL_element_) {
        case (omfl::Bool):
          std::cout << (section.bool_stored_value_ ? "true" : "false");
          break;
        case (omfl::Int):
          std::cout << section.int_stored_value_;
          break;
        case (omfl::Float):
          std::cout << section.float_stored_value_;
          break;
        case (omfl::String):
          std::cout << "\"" + section.string_stored_value_ + "\"";
          break;
        case (omfl::Array):
          std::cout << "[";
          for (auto& it : section.array_) {
            PrintSection(it);
            std::cout << " ";
          }
          std::cout << "]";
          break;
      }
      break;
    case (omfl::Section):
      for (auto it : section.sub_sections_) {
        std::cout << it.first << " : ";
        PrintSection(it.second);
        std::cout << std::endl;
      }
      break;
  }
}