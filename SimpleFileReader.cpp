/*
 * Copyright ©2025 Travis McGaha.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Pennsylvania
 * CIT 5950 for use solely during Spring Semester 2025 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "SimpleFileReader.hpp"

SimpleFileReader::SimpleFileReader(const std::string& fname)
    : fd_(-1), good_(false) {
  open_file(fname);
}

SimpleFileReader::~SimpleFileReader() {
  close_file();
}

void SimpleFileReader::open_file(const std::string& fname) {
  // Close existing file if one is open
  close_file();

  // Open the file for reading only
  fd_ = open(fname.c_str(), O_RDONLY);

  // Check if file was opened successfully
  if (fd_ >= 0) {
    good_ = true;
  } else {
    good_ = false;
  }
}

void SimpleFileReader::close_file() {
  if (fd_ >= 0) {
    close(fd_);
    fd_ = -1;
  }
  good_ = false;
}

char SimpleFileReader::get_char() {
  if (!good_ || fd_ < 0) {
    return EOF;
  }

  char c;
  ssize_t bytes_read = read(fd_, &c, 1);

  // If we couldn't read a character, we're at EOF
  if (bytes_read <= 0) {
    good_ = false;
    return EOF;
  }

  return c;
}

std::optional<std::string> SimpleFileReader::get_chars(size_t n) {
  if (!good_ || fd_ < 0) {
    return std::nullopt;
  }

  // If n is 0, return empty string
  if (n == 0) {
    return std::string();
  }

  // Buffer to store characters
  std::string result;
  result.resize(n);

  ssize_t bytes_read = read(fd_, &result[0], n);

  // If we couldn't read any characters, we're at EOF
  if (bytes_read <= 0) {
    good_ = false;
    return std::nullopt;
  }

  // If we read fewer characters than requested, we've hit EOF
  if (static_cast<size_t>(bytes_read) < n) {
    good_ = false;
  }

  // Adjust the size to match what was actually read
  result.resize(bytes_read);

  return result;
}

int SimpleFileReader::tell() const {
  if (fd_ < 0) {
    return -1;
  }

  // Get current position in file
  return lseek(fd_, 0, SEEK_CUR);
}

void SimpleFileReader::rewind() {
  if (fd_ >= 0) {
    lseek(fd_, 0, SEEK_SET);
    good_ = true;
  }
}

bool SimpleFileReader::good() const {
  return good_ && (fd_ >= 0);
}