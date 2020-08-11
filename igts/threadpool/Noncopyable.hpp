#ifndef __crypto_NONCOPYABLE_H__
#define __crypto_NONCOPYABLE_H__



class Noncopyable{
protected:
	Noncopyable(){ }
	~Noncopyable(){ }
private:
	Noncopyable(const Noncopyable &);
	Noncopyable & operator=(const Noncopyable &);
};


#endif
