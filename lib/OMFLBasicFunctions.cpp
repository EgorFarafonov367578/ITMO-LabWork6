#include "parser.h"

#include <sstream>
#include <iostream>

omfl::OMFL ERRORForOMFL("");

bool omfl::OMFL::IsBool() const {
  switch (class_of_OMFL_object_) {
    case (Class_of_Selection::Section):return false;
    case (Class_of_Selection::Element):
      if (class_of_OMFL_element_ == Class_of_Element_of_OMFL::Bool) {
        return true;
      } else {
        return false;
      }
  }
}

bool omfl::OMFL::IsInt() const {
  switch (class_of_OMFL_object_) {
    case (Class_of_Selection::Section):return false;
    case (Class_of_Selection::Element):
      if (class_of_OMFL_element_ == Class_of_Element_of_OMFL::Int) {
        return true;
      } else {
        return false;
      }
  }
}

bool omfl::OMFL::IsFloat() const {
  switch (class_of_OMFL_object_) {
    case (Class_of_Selection::Section):return false;
    case (Class_of_Selection::Element):
      if (class_of_OMFL_element_ == Class_of_Element_of_OMFL::Float) {
        return true;
      } else {
        return false;
      }
  }
}

bool omfl::OMFL::IsString() const {
  switch (class_of_OMFL_object_) {
    case (Class_of_Selection::Section):return false;
    case (Class_of_Selection::Element):
      if (class_of_OMFL_element_ == Class_of_Element_of_OMFL::String) {
        return true;
      } else {
        return false;
      }
  }
}

bool omfl::OMFL::IsArray() const {
  switch (class_of_OMFL_object_) {
    case (Class_of_Selection::Section):return false;
    case (Class_of_Selection::Element):
      if (class_of_OMFL_element_ == Class_of_Element_of_OMFL::Array) {
        return true;
      } else {
        return false;
      }
  }
}

bool omfl::OMFL::AsBool() const {
  switch (class_of_OMFL_object_) {
    case (Class_of_Selection::Section):return false;
    case (Class_of_Selection::Element):
      if (class_of_OMFL_element_ == Class_of_Element_of_OMFL::Bool) {
        return bool_stored_value_;
      } else {
        return false;
      }
  }
}

long long int omfl::OMFL::AsInt() const {
  switch (class_of_OMFL_object_) {
    case (Class_of_Selection::Section):return -1;
    case (Class_of_Selection::Element):
      if (class_of_OMFL_element_ == Class_of_Element_of_OMFL::Int) {
        return int_stored_value_;
      } else {
        return -1;
      }
  }
}

double omfl::OMFL::AsFloat() const {
  switch (class_of_OMFL_object_) {
    case (Class_of_Selection::Section):return -1;
    case (Class_of_Selection::Element):
      if (class_of_OMFL_element_ == Class_of_Element_of_OMFL::Float) {
        return float_stored_value_;
      } else {
        return -1;
      }
  }
}

const std::string& omfl::OMFL::AsString() const {
  switch (class_of_OMFL_object_) {
    case (Class_of_Selection::Section):
      throw std::invalid_argument("received negative value");
    case (Class_of_Selection::Element):
      if (class_of_OMFL_element_ == Class_of_Element_of_OMFL::String) {
        return string_stored_value_;
      } else {
        throw std::invalid_argument("received negative value");
      }
  }
}

bool omfl::OMFL::AsBoolOrDefault(bool default_value) const {
  switch (class_of_OMFL_object_) {
    case (Class_of_Selection::Section):return default_value;
    case (Class_of_Selection::Element):
      if (class_of_OMFL_element_ == Class_of_Element_of_OMFL::Bool) {
        return bool_stored_value_;
      } else {
        return default_value;
      }
    case (Class_of_Selection::Error):return default_value;
  }
}

long long int omfl::OMFL::AsIntOrDefault(long long int default_value) const {
  switch (class_of_OMFL_object_) {
    case (Class_of_Selection::Section):return default_value;
    case (Class_of_Selection::Element):
      if (class_of_OMFL_element_ == Class_of_Element_of_OMFL::Int) {
        return int_stored_value_;
      } else {
        return default_value;
      }
    case (Class_of_Selection::Error):return default_value;
  }
}

double omfl::OMFL::AsFloatOrDefault(double default_value) const {
  switch (class_of_OMFL_object_) {
    case (Class_of_Selection::Section):return default_value;
    case (Class_of_Selection::Element):
      if (class_of_OMFL_element_ == Class_of_Element_of_OMFL::Float) {
        return float_stored_value_;
      } else {
        return default_value;
      }
    case (Class_of_Selection::Error):return default_value;
  }
}

const std::string& omfl::OMFL::AsStringOrDefault(const std::string& default_value) const {
  switch (class_of_OMFL_object_) {
    case (Class_of_Selection::Section):
      return default_value;
    case (Class_of_Selection::Element):
      if (class_of_OMFL_element_ == Class_of_Element_of_OMFL::String) {
        return string_stored_value_;
      } else {
        return default_value;
      }
    case (Class_of_Selection::Error):
      return default_value;
  }
}

const omfl::OMFL& omfl::OMFL::Get(const std::string& sub_section_name) const {
  std::vector<std::string> sub_sections;
  std::string next_section = "";
  for (char c : sub_section_name) {
    if (c == '.') {
      sub_sections.push_back(next_section);
      next_section = "";
    } else {
      next_section.push_back(c);
    }
  }
  if (next_section != "") {
    sub_sections.push_back(next_section);
    next_section = "";
  }
  const omfl::OMFL* ans = this;
  for (int i = 0; i < sub_sections.size(); i++) {
    if (ans->sub_sections_.find(sub_sections[i]) == ans->sub_sections_.end()) {
      throw std::invalid_argument("received negative value");
    } else {
      const auto it = ans->sub_sections_.find(sub_sections[i]);
      ans = &it->second;
    }
  }
  return (*ans);
}

const omfl::OMFL& omfl::OMFL::operator[](uint32_t number_of_element_in_array) const {
  if (class_of_OMFL_object_ == omfl::Class_of_Selection::Element) {
    if (class_of_OMFL_element_ == omfl::Class_of_Element_of_OMFL::Array) {
      if (number_of_element_in_array < array_.size()) return array_[number_of_element_in_array];
      return (ERRORForOMFL);
    } else {
      throw std::invalid_argument("received negative value");
    }
  } else {
    throw std::invalid_argument("received negative value");
  }
}

bool omfl::OMFL::valid() const {
  return (class_of_OMFL_object_ != Class_of_Selection::Error);
}