#ifndef _FLASHQUERY_DOCUMENT_HPP
#define _FLASHQUERY_DOCUMENT_HPP

#include <string_view>

namespace flashquery {

class Document {
public:
    explicit Document(const std::string_view &html);
};

} // namespace flashquery

#endif // _FLASHQUERY_DOCUMENT_HPP
