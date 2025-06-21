#ifndef FLASHQUERY_DOCUMENT_HPP
#define FLASHQUERY_DOCUMENT_HPP

#include <string_view>

namespace flashquery {

class Document {
public:
    explicit Document(const std::string_view &html);
};

} // namespace flashquery

#endif // FLASHQUERY_DOCUMENT_HPP
