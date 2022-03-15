#pragma once

class Observer
    
{
    
public:
    
        virtual ~Observer();        // Destructor
    
        virtual void Notify(Observee * observee) {};
    
protected:
    
        //constructor is protected because this class is abstract, it’s only meant to be inherited!
        
        Observer();
    
private:
    
        // -------------------------
        
        // Disabling default copy constructor and default
        
        // assignment operator.
        
        // -------------------------
        
        Observer(const Observer & yRef);
    
        Observer & operator=(const Observer & yRef);
    
};
