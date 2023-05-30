#include "config/Line.hpp"

namespace config {

Line::Line(Line const& src) : m_line(src.m_line), m_type(src.m_type) {}

Line::Line(std::string const& line) : m_line(ft::string::trim(line)) {
    if (m_line.empty() || m_line[0] == '#') { m_type = Empty; }
    else if (m_line[0] == '}') { m_type = End; }
    else if (m_line.find('{') != std::string::npos) {
        if (m_line.find("location") != std::string::npos) { m_type = Route; }
        else if (m_line.find("server") != std::string::npos) { m_type = Block; }
        else { m_type = Unknown; }
    }
    else if (m_line.find(';') != std::string::npos) {
        // TODO: refactor with other directives
        m_type = Listen;
    }
    else { m_type = Unknown; }
}

Line::~Line(void) {}

Line& Line::operator=(Line const& rhs) {
    if (this != &rhs) {
        m_line = rhs.m_line;
        m_type = rhs.m_type;
    }
    return (*this);
}

std::string Line::getLine(void) const { return (m_line); }

LineType Line::getType(void) const { return (m_type); }

std::vector<std::string> Line::format(void) const {
    std::vector<std::string> subs;
    if (m_type == Empty || m_type == Unknown) { return (subs); }

    // remove everything after a comment
    size_t      pos = m_line.find('#');
    std::string str =
        (pos == std::string::npos) ? m_line : m_line.substr(0, pos);
    str = ft::string::trim(str);

    // check if the line is start/end of block or of route
    if ((m_type == Block || m_type == Route) &&
        str.find('{') != str.size() - 1) {
        return (subs);
    }
    else if (m_type == End && str != "}") { return (subs); }
    else if (m_type > End && str.find(';') != str.size() - 1) { return (subs); }

    str = (str.find(';') != std::string::npos) ? (str.substr(0, str.size() - 1))
                                               : str;

    subs = ft::string::split(str, " \t");
    return (subs);
}

} // namespace config