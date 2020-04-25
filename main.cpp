#include <iostream>
#include <cmath>
using namespace std;

class Polynomial{
protected:

    class Term{
    protected:
    int coefficient;
    int exponent;
    Term *next;
    Term (int exp, int coeff, Term *n):exponent(exp),coefficient(coeff),next(n){};
    friend class Polynomial;
    public:
    Term *getNext(){return next;}
    int getExp(){return exponent;}
    int getCo(){return coefficient;}
    };
    Term *head;
public:
    Polynomial():head(NULL){}
    Polynomial (const Polynomial &p){
        if (p.head==NULL) head=NULL;
        else {
            head=new Term (p.head->exponent,p.head->coefficient,NULL);
            Term *curr_p=p.head->next;
            Term *curr=head;
            while (curr_p!=NULL){
                curr->next=new Term(curr_p->exponent,curr_p->coefficient,NULL);
                curr=curr->next;
                curr_p=curr_p->next;
            }
        }
    }
    ~Polynomial(){
        Term *destructor=NULL;
        while (head!=NULL){
            destructor=head;
            head=head->next;
            delete destructor;
        }
    }

    Polynomial & operator = (const Polynomial &p){
        Term *destructor=NULL;
        while (head!=NULL){
            destructor=head;
            head=head->next;
            delete destructor;
        }
        if (p.head==NULL) head=NULL;
        else {
            head=new Term (p.head->exponent,p.head->coefficient,NULL);
            Term *curr_p=p.head->next;
            Term *curr=head;
            while (curr_p!=NULL){
                curr->next=new Term(curr_p->exponent,curr_p->coefficient,NULL);
                curr=curr->next; 
                curr_p=curr_p->next;
            }
        }
        return *this;
    }

    void addTerm (int exp,int coeff){
        if (coeff==0) return ;
        if (head==NULL || head->exponent< exp) head=new Term(exp, coeff, head);
        else if (head->exponent==exp){
            head->coefficient+=coeff;
            if (head->coefficient==0){
                Term *temp=head;
                head=head->next;
                delete temp;
            }
        }
        else {
            Term *curr=head;
            Term *next=curr->next;
            while (next!=NULL && exp<next->exponent){
                curr=next;
                next=next->next;
            }
            if (next==NULL) curr->next= new Term (exp,coeff,next);  
            else if (exp > next->exponent) curr->next= new Term (exp,coeff,next);  
            else if (exp == next->exponent){  
                next->coefficient+=coeff;  
                if (next->coefficient==0){  
                    Term *trash=next;  
                    curr->next=next->next;  
                    delete next;  
                }  
            } 
        }
    }
    double evaluate (int x){
        double result=0;
        Term *curr=head;
        while (curr!=NULL) {
            result+=pow(x,curr->exponent)*(curr->coefficient);
            curr=curr->next;
        }
        return result;
    }

     friend Polynomial operator + (const Polynomial &p, const Polynomial &q){
        if (p.head==NULL && q.head!=NULL){Polynomial Result(q); return Result;}
        if (p.head!=NULL && q.head==NULL){Polynomial Result(p); return Result;}
        if (p.head->getExp() > q.head->getExp()){
            Polynomial Result(p);
            for (Term *curr=q.head; curr!=NULL; curr=curr->getNext())
                Result.addTerm(curr->getExp(),curr->getCo());
            return Result;
        }
        else{
            Polynomial Result(q);
            for (Term *curr=p.head; curr!=NULL; curr=curr->getNext())
                Result.addTerm(curr->getExp(),curr->getCo());
            return Result;
        }
    }
    friend Polynomial operator * (const Polynomial &p, const Polynomial &q){
        Polynomial result;
        Term *curr_res=result.head;
        Term *curr_p=p.head;
        Term *curr_q=q.head;
        while (curr_p!=NULL){
            while (curr_q!=NULL){
                result.addTerm(curr_p->getExp()+curr_q->getExp(),curr_p->getCo()*curr_q->getCo());
                curr_q=curr_q->getNext();
            }
            curr_p=curr_p->getNext();
            curr_q=q.head;
        }
        return result;
    }

    friend std::ostream & operator << (std::ostream &out , const Polynomial &p){
        if (p.head==NULL) out<<0;
        else{
            Term *curr=p.head;
            if (curr->getCo()<0) out<<"- ";
            if (abs(curr->getCo())!=1 || curr->getExp()==0) out<<abs(curr->getCo());
            if (curr->getExp()>1) out<<"x^"<<curr->getExp();
            else if (curr->getExp()==1) out<<'x';
            curr=curr->getNext();
            while (curr!=NULL){
                if (curr->getCo()<0) out<<" - ";
                else out<<" + ";
                if (abs(curr->getCo())!=1 || curr->getExp()==0) out<<abs(curr->getCo());
                if (curr->getExp()>1) out<<"x^"<<curr->getExp();
                else if (curr->getExp()==1) out<<"x";
                curr=curr->getNext();
            }
        }
        return out;
    }

};


int main(){
    Polynomial p;
    p.addTerm(2,3);
    p.addTerm(4,-1);
    p.addTerm(2,-1);
    p.addTerm(1,6);
    p.addTerm(0,4);
    cout<<p<<endl;
    cout<<p.evaluate(0)<<endl;
    cout<<p.evaluate(1)<<endl;
    cout<<p.evaluate(5)<<endl;
    cout<<p.evaluate(10)<<endl;

    Polynomial q;
    q.addTerm(0,-3);
    q.addTerm(1,5);
    q.addTerm(2,-10);
    q.addTerm(2,4);
    q.addTerm(0,3);
    cout<<q<<endl;
    cout<<q.evaluate(0)<<endl;
    cout<<q.evaluate(1)<<endl;
    cout<<q.evaluate(5)<<endl;
    cout<<q.evaluate(10)<<endl;

    cout<<p<<endl<<q<<endl;
    Polynomial sum(p+q);
    cout<<sum<<endl;
    cout<<sum.evaluate(0)<<endl;
    cout<<sum.evaluate(1)<<endl;
    cout<<sum.evaluate(5)<<endl;
    cout<<sum.evaluate(10)<<endl;
    cout<<p*q<<endl;

    return 0;

}
