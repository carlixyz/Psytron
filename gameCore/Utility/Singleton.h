#ifndef SINGLETON_H
#define	SINGLETON_H

//Singleton class
template<typename T> class Singleton
{
  public:
    static T& Get()// Return Singleton instance
    {
      //Return the static reference to itself.
      //To be used instead the constructor.
      static T Instance;
      return Instance;
    }


};

#endif	

