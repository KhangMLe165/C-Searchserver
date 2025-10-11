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

#include "BufferedFileReader.hpp"

// one provided function since this one has funky syntax
// it is just a wrapper around the good function though.
BufferedFileReader::operator bool() const {
  return this->good();
}

BufferedFileReader::BufferedFileReader(const std::string& fname)
    : curr_length_(0), curr_index_(0), fd_(-1), good_(false) {
  open_file(fname);
}

BufferedFileReader::~BufferedFileReader() {
  close_file();
}

BufferedFileReader::BufferedFileReader(BufferedFileReader&& other)
    : curr_length_(other.curr_length_),
      curr_index_(other.curr_index_),
      fd_(other.fd_),
      good_(other.good_) {
  // Copy the buffer data
  for (size_t i = 0; i < BUF_SIZE; i++) {
    buffer_[i] = other.buffer_[i];
  }

  // Reset the moved-from object
  other.fd_ = -1;
  other.good_ = false;
  other.curr_length_ = 0;
  other.curr_index_ = 0;
}

BufferedFileReader& BufferedFileReader::operator=(BufferedFileReader&& other) {
  // Check for self-assignment
  if (this == &other) {
    return *this;
  }

  // Close any existing file
  close_file();

  // Move resources from other
  fd_ = other.fd_;
  good_ = other.good_;
  curr_length_ = other.curr_length_;
  curr_index_ = other.curr_index_;

  // Copy the buffer data explicitly
  for (size_t i = 0; i < BUF_SIZE; i++) {
    buffer_[i] = other.buffer_[i];
  }

  // Reset the moved-from object
  other.fd_ = -1;
  other.good_ = false;
  other.curr_length_ = 0;
  other.curr_index_ = 0;

  return *this;
}

void BufferedFileReader::open_file(const std::string& fname) {
  // Close existing file if one is open
  close_file();

  // Open the file for reading only
  fd_ = open(fname.c_str(), O_RDONLY);

  // Check if file was opened successfully
  if (fd_ >= 0) {
    good_ = true;
    curr_length_ = 0;
    curr_index_ = 0;
  } else {
    good_ = false;
  }
}

void BufferedFileReader::close_file() {
  if (fd_ >= 0) {
    close(fd_);
    fd_ = -1;
  }
  good_ = false;
  curr_length_ = 0;
  curr_index_ = 0;
}

void BufferedFileReader::fill_buffer() {
  if (fd_ < 0) {
    good_ = false;
    return;
  }

  // Reset buffer indices
  curr_index_ = 0;

  // Try to read BUF_SIZE characters into buffer
  ssize_t bytes_read = read(fd_, buffer_.data(), BUF_SIZE);

  if (bytes_read <= 0) {
    // EOF or error
    curr_length_ = 0;
    good_ = false;
  } else {
    curr_length_ = static_cast<size_t>(bytes_read);
    good_ = true;
  }
}

char BufferedFileReader::get_char() {
  if (!good_ || fd_ < 0) {
    return EOF;
  }

  // If buffer is empty or we've read all characters in it,
  // refill the buffer
  if (curr_length_ == 0 || curr_index_ >= curr_length_) {
    fill_buffer();
    if (curr_length_ == 0) {  // EOF or error
      return EOF;
    }
  }

  // Return next character from buffer
  return buffer_[curr_index_++];
}

std::optional<std::string> BufferedFileReader::get_token(
    const std::string& delims) {
  if (!good_ || fd_ < 0) {
    return std::nullopt;
  }

  // String to build the token
  std::string token;

  // Get the first character
  char c = get_char();

  // Check if we hit EOF right away
  if (c == EOF) {
    return std::nullopt;
  }

  // Check if first character is a delimiter
  if (delims.find(c) != std::string::npos) {
    // Return empty token when starting with a delimiter
    return token;
  }

  // Add first character to token
  token.push_back(c);

  // Continue reading until we hit a delimiter or EOF
  while (true) {
    c = get_char();

    // Check if we hit EOF
    if (c == EOF) {
      return token;
    }

    // Check if current character is a delimiter
    if (delims.find(c) != std::string::npos) {
      return token;
    }

    // Add character to token
    token.push_back(c);
  }
}

int BufferedFileReader::tell() const {
  if (fd_ < 0) {
    return -1;
  }

  // Get the current position in the file
  int file_pos = lseek(fd_, 0, SEEK_CUR);

  if (file_pos < 0) {
    return -1;
  }

  // Adjust for characters we've read into the buffer but not consumed yet
  return file_pos - (curr_length_ - curr_index_);
}

void BufferedFileReader::rewind() {
  if (fd_ >= 0) {
    // Reset file position to beginning
    lseek(fd_, 0, SEEK_SET);

    // Reset buffer state
    curr_length_ = 0;
    curr_index_ = 0;
    good_ = true;
  }
}

bool BufferedFileReader::good() const {
  return good_ && (fd_ >= 0);
}