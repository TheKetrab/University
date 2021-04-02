// cpp stl Bartlomiej Grochowski 300951
// zadanie 1 - odwrotna notacja polska

#include <iostream>
#include <exception>
#include <list>
#include <stack>
#include <queue>

using namespace std;

// ***************************** //
// ----- ----- TOKEN ----- ----- //
// ***************************** //
struct token {

    enum class type { ERR, NUM, OP, PAR };

    type t;
    int val;

    token(type t, int val) : t(t), val(val) {}
    int priority();
    friend ostream& operator<<(ostream& os, const token& tk);
};

using tk = token::type;
int token::priority() {

    if (t != type::OP)
        return 0;

    switch ((char)val) {
        case '*':
        case '/':
            return 1;
    }

    return 0;
}

ostream& operator<<(ostream& os, const token& tk)
{
    switch (tk.t) {
        case token::type::ERR: os << "UNKNOWN"; break;
        case token::type::NUM: os << tk.val; break;
        case token::type::PAR: os << ((tk.val == 0) ? "(" : ")"); break;
        case token::type::OP:  os << (char)tk.val; break;
    }

    return os;
}

// ***************************** //
// ----- ----- LEXER ----- ----- //
// ***************************** //
#define CASE_OPERATOR case '+': case '-': case '*': case '/':
#define CASE_WHITESPACE case ' ': case '\t': case '\n':
#define CASE_DIGIT case '0': \
    case '1': case '2': case '3': \
    case '4': case '5': case '6': \
    case '7': case '8': case '9': \

class lexer {

    list<token> l;           // wynikowa lista tokenow
    int par_depth   = 0;     // glebokosc nawiasow
    int number      = 0;     // tworzona liczba
    bool digit_mode = false; // tryb tworzenia liczby

    void validate(bool finally = false);
    pair<token,token> last_two_tokens();
    void create_number();

public:
    lexer() = default;
    list<token> tokenize(string expr);
};

pair<token,token> lexer::last_two_tokens() {

    if (l.size() == 0)
        return { token(tk::ERR,0), token(tk::ERR,0) };

    else if (l.size() == 1)
        return { token(tk::ERR,0), l.back() };

    else {
        auto it = l.end();
        it--; it--;
        return { *it, l.back() };
    }
}

void lexer::create_number() {
    l.push_back(token(tk::NUM,number));
    digit_mode = false;
    number = 0;
}

#define ASSERT_NEXTTO(condition) ASSERT(condition,"Invalid tokens next to each other!")

#define ASSERT(condition,err_msg) { \
    if (!(condition)) \
        throw invalid_argument(err_msg); \
}

void lexer::validate(bool finally) {

    // NOTHING TO VALIDATE
    if (l.size() == 0)
        return;

    // VALIDATE AT THE END
    if (finally) {
        ASSERT(par_depth == 0, "Parenthesis not closed.");
        ASSERT(l.back().t != tk::OP, "Expression ended by operator.");
    }

    auto last = last_two_tokens();
    token t0 = last.second; // ostatni
    token t1 = last.first; // przedostatni

    // VALIDATE AFTER CHARACTER
    ASSERT(par_depth >= 0, "Extra closing parenthesis found!");
    ASSERT(t0.t != tk::ERR, "Unknown character: " + string(1,(char)(t0.val)));

    if (l.size() == 1)
        return;

    // VALIDATE TWO LAST TOKENS
    // --------------------------------------------------
    // n n = ERR_A | o n         | ( n       | ) n ERR_G
    // n o         | o o = ERR_C | ( o ERR_E | ) o 
    // n ( = ERR_B | o (         | ( (       | ) ( ERR_H
    // n )         | o ) = ERR_D | ( ) ERR_F | ) )
    
    ASSERT_NEXTTO(!(t1.t == tk::NUM && t0.t == tk::NUM));                               // ERR_A
    ASSERT_NEXTTO(!(t1.t == tk::NUM && t0.t == tk::PAR && t0.val == 0));                // ERR_B
    ASSERT_NEXTTO(!(t1.t == tk::OP && t0.t == tk::OP));                                 // ERR_C
    ASSERT_NEXTTO(!(t1.t == tk::OP && t0.t == tk::PAR && t0.val == 1));                 // ERR_D
    ASSERT_NEXTTO(!(t1.t == tk::PAR && t1.val == 0 && t0.t == tk::OP));                 // ERR_E  
    ASSERT_NEXTTO(!(t1.t == tk::PAR && t1.val == 0 && t0.t == tk::PAR && t0.val == 1)); // ERR_F  
    ASSERT_NEXTTO(!(t1.t == tk::PAR && t1.val == 1 && t0.t == tk::NUM));                // ERR_G  
    ASSERT_NEXTTO(!(t1.t == tk::PAR && t1.val == 1 && t0.t == tk::PAR && t0.val == 0)); // ERR_H  

}

list<token> lexer::tokenize(string expr) {

    // czyszczenie stanu
    l.clear(), par_depth = 0, number = 0, digit_mode = false;

    int n = expr.size();

    for (int i=0; i<=n; i++) {

        switch (expr[i]; char c = expr[i]) {

        case '\0': {
            if (digit_mode) create_number();
            break;
        }

        CASE_WHITESPACE {
            if (digit_mode) create_number();
            break;
        }

        CASE_OPERATOR {
            if (digit_mode) create_number();
            l.push_back(token(tk::OP,c));
            break;
        }

        CASE_DIGIT {
            if (digit_mode) { number = number*10 + (c - '0'); }
            else { number = c - '0'; digit_mode = true; }
            break;
        }

        case '(': {
            if (digit_mode) create_number();
            par_depth++;
            l.push_back(token(tk::PAR,0));
            break;
        }

        case ')': {
            if (digit_mode) create_number();
            par_depth--;
            l.push_back(token(tk::PAR,1));
            break;
        }

        default: {
            if (digit_mode) create_number();
            l.push_back(token(tk::ERR,c));
        }

        }

        validate();
    }

    validate(true);
    return l;
}

// ***************************** //
// --- ROTATE POLISH NOTATION -- //
// ***************************** //
list<token> to_rpn(const list<token> &tokens) {

    stack<token> stack;
    queue<token> input;
    queue<token> output;
    
    // copy
    for (auto& tk : tokens) input.push(tk);

    // rpn algorithm
    while (!input.empty()) {

        token cur_tok = input.front();
        input.pop();

        switch (cur_tok.t) {

        case tk::NUM:
            output.push(cur_tok);
            break;
        case tk::OP: {

            while (!stack.empty()) {

                if (stack.top().t != tk::OP) break;

                token op_on_top = stack.top();

                if (op_on_top.priority() >= cur_tok.priority()) {
                    output.push(stack.top());
                    stack.pop();
                } else break;
            }

            stack.push(cur_tok);
            break;
        }
        case tk::PAR: {

            // '('
            if (cur_tok.val == 0) {
                stack.push(cur_tok);
            }

            // ')'
            else {

                while (!stack.empty()) {
                    
                    token tok_on_top = stack.top();
                    if (tok_on_top.t == tk::PAR) break;

                    output.push(tok_on_top);
                    stack.pop();
                }

                if (stack.empty())
                    throw invalid_argument("Parenthesis problem.");

                stack.pop();
            }

            break;
        }
        default: {
            throw invalid_argument("Unknown token.");
        }
        }
    }

    // zrzuc to co zostalo ze stosu
    while (!stack.empty()) {
        auto top_tok = stack.top();
        if (top_tok.t == tk::PAR)
            throw std::invalid_argument("Parenthesis problem.");

        output.push(top_tok);
        stack.pop();
    }

    // przekonwertuj do listy
    list<token> res;
    while (!output.empty()) {
        token tok = output.front();
        res.push_back(tok);
        output.pop();
    }

    return res;
}


// ******************************* //
// ----- ----- PROGRAM ----- ----- //
// ******************************* //

void print_tokens(const list<token> &tks) {
    for (auto& t : tks)
        cout << t << " ";
    cout << endl;
}

void test(string expression) {
    
    lexer l;
    auto tokens = l.tokenize(expression);
    auto rpn_tokens = to_rpn(tokens);
    print_tokens(rpn_tokens);
}

int main () {

    lexer l;

    try {
        test("1+2\t* 3");
        test("(1+2)*3");
        test("8*2+(11*1-4)/3");
    }
    catch (exception &e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}