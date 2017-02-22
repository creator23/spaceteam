// Using parser found at: http://www.boost.org/doc/libs/1_62_0/doc/html/boost_asio/example/cpp11/http/server/request_parser.hpp
// For ease of parsing the request given by connection.h
// Will use request_handler to find the right data.

#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

#include <tuple>
#include "request.h"

class Request;

// parser for incoming requests.
class RequestParser {
public:
  // Construct ready to parse the request method.
  RequestParser();

  // Resets the state of the parser. 
  void reset();

  enum result_type { good, bad, indeterminate };

  // Parse some data. The enum return value is good when a complete request has
  // been parsed, bad if the data is invalid, indeterminate when more data is
  // required. The InputIterator return value indicates how much of the input
  // has been consumed.
  template <typename InputIterator>
  std::tuple<result_type, InputIterator> parse(Request& req,
      InputIterator begin, InputIterator end) {
    while (begin != end) {
      result_type result = consume(req, *begin++);
      if (result == good || result == bad)
        return std::make_tuple(result, begin);
    }
    return std::make_tuple(indeterminate, begin);
  }

private:
  /// Handle the next character of input.
  result_type consume(Request& req, char input);

  /// Check if a byte is an HTTP character.
  static bool is_char(int c);

  /// Check if a byte is an HTTP control character.
  static bool is_ctl(int c);

  /// Check if a byte is defined as an HTTP tspecial character.
  static bool is_tspecial(int c);

  /// Check if a byte is a digit.
  static bool is_digit(int c);

  /// The current state of the parser.
  enum state {
    method_start,
    method,
    handler_path_start,
    handler_path,
    file_path,
    uri,
    http_version_h,
    http_version_t_1,
    http_version_t_2,
    http_version_p,
    http_version_slash,
    http_version_major_start,
    http_version_major,
    http_version_minor_start,
    http_version_minor,
    expecting_newline_1,
    header_line_start,
    header_lws,
    header_name,
    space_before_header_value,
    header_value,
    expecting_newline_2,
    expecting_newline_3
  } state_;
};

#endif // REQUEST_PARSER_H
