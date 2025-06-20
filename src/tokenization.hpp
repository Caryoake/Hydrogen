#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <sstream>

using namespace std;

enum class TokenType {
    exit,          // Exit keyword
    int_lit,       // Integer literal
    semi,          // Semicolon ;
    open_paren,    // Opening parenthesis (
    close_paren,   // Closing parenthesis )
    ident,         // Identifier
    let,           // Let keyword
    eq,            // Equal sign =
    plus,          // Plus operator +
    star,          // Multiplication operator *
    minus,           // Subtraction operator -
    fslash,           // Division operator /
    open_curly,    // Opening curly brace {
    closed_curly,   // Closing curly brace }
    if_             // if statment
};


optional<int> bin_prec(TokenType type) {
    switch (type) {
        case TokenType::plus:
        case TokenType::minus:
            return 0;
        case TokenType::star:
        case TokenType::fslash:
            return 1;
        default:
            return{};
    }
}

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

                }
             else if(buf == "let")
             {
                 tokens.push_back({ .type =TokenType::let});
                 buf.clear();

             }
             else if(buf == "if")
             {
                 tokens.push_back({ .type =TokenType::if_});
                 buf.clear();
             }
             else
                {
                    tokens.push_back({ .type = TokenType::ident, .value = buf});
                    buf.clear();

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

            }
            else if(peek().value() == ';'){
                kazhikk();
                tokens.push_back({ .type = TokenType::semi});

            }
            else if(peek().value() == '+')
            {
                kazhikk();
                tokens.push_back({ .type = TokenType::plus});

            }
            else if(peek().value() == '-')
            {
                kazhikk();
                tokens.push_back({ .type = TokenType::minus});

            }
            else if(peek().value() == '/')
            {
                kazhikk();
                tokens.push_back({ .type = TokenType::fslash});

            }
            else if(peek().value() == '{')
            {
                kazhikk();
                tokens.push_back({ .type = TokenType::open_curly});

            }
            else if(peek().value() == '}')
            {
                kazhikk();
                tokens.push_back({ .type = TokenType::closed_curly});

            }
            else if(peek().value() == '*') {
                kazhikk();
                tokens.push_back({.type = TokenType::star});

            }
            else if(isspace(peek().value())){
                kazhikk();

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