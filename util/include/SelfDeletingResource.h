#ifndef GENG_SELF_DELETING_RESOURCE_H
#define GENG_SELF_DELETING_RESOURCE_H

/**
When the number of references to an instance of this interface reach zero, 
the object is expected to delete itself as the last operation in releaseReference().

Classes implementing this interface are expected to have a factory method,
such as load(), that return a pointer to the object.  This allows for the return of
the same pointer for the same argument to load().

Classes implementing this interface should declare their destructors as private to
disable outside calls to delete on pointers to instances of the class.
*/
class SelfDeletingResource
{
public:
    virtual void acquireReference() =0;
    virtual void releaseReference() =0;
};

#endif
