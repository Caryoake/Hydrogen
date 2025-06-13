#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <sstream>

using namespace std;

enum class TokenType{exit , int_lit , semi, open_paren, close_paren, ident, let, eq, plus};

struct Token{
    TokenType type;
    optional<string> value{};
};

class Tokenizer{
public:

    inline explicit Tokenizer(const string& src)
        : m_src(src) {
    }

    inline vector<Token> tokenize(){
        vector<Token> tokens;
        string buf;
        while (peek().has_value())
        {
            if (isalpha(peek().value()))
            {
             buf.push_back(kazhikk());
             while (peek().has_value() && isalnum(peek().value()))
             {
                 buf.push_back(kazhikk());
             }
             if (buf == "exit")
                {
                    tokens.push_back({.type = TokenType::exit});
                    buf.clear();
                    continue;
                }
             else if(buf == "let")
             {
                 tokens.push_back({ .type =TokenType::let});
                 buf.clear();
                 continue;
             }
             else
                {
                    tokens.push_back({ .type = TokenType::ident, .value = buf});
                    buf.clear();
                    continue;
                }

            }

            else if (isdigit(peek().value()))
            {
                buf.push_back(kazhikk());
                while(peek().has_value() && isdigit(peek().value()))
                {
                    buf.push_back(kazhikk());
                }
                tokens.push_back({ .type = TokenType::int_lit, .value = buf});
                buf.clear();

            }
            else if(peek().value() == '(')
            {
                kazhikk();
                tokens.push_back({ .type = TokenType::open_paren});
            }
            else if(peek().value() == ')')
            {
                kazhikk();
                tokens.push_back({ .type = TokenType::close_paren});
            }
            else if(peek().value() == '='){
                kazhikk();
                tokens.push_back({ .type = TokenType::eq});
                continue;
            }
            else if(peek().value() == ';'){
                kazhikk();
                tokens.push_back({ .type = TokenType::semi});
                continue;
            }
            else if(peek().value() == '+')
            {
                kazhikk();
                tokens.push_back({ .type = TokenType::plus});
                continue;
            }
            else if(isspace(peek().value())){
                kazhikk();
                continue;
            }
            else{
                cerr<<"language il illatha saanam itta engane mansilavum"<<endl;
                exit(EXIT_FAILURE);
            }
        }
        m_index = 0;
        return tokens;
    }


private:
    const string m_src;
    size_t m_index = 0;

    [[nodiscard]] inline optional<char> peek(int offset = 0) const
    {
        if(m_index + offset >= m_src.length())
        {
            return {};
        }
        else
        {
             return m_src.at(m_index + offset);
        }

    }

    inline char kazhikk()
    {
        return m_src.at(m_index++);
    }



};