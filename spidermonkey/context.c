#include "context.h"
#include "runtime.h"

PyObject*
Context_new(PyTypeObject* type, PyObject* args, PyObject* kwargs)
{
    Context* self;
    Runtime* runtime = NULL;
    PyObject* rt = NULL;
    PyObject* root = NULL;
    PyObject* f = NULL;
    
    fprintf(stderr, "NEW CONTEXT!\n");
    
    static char *kwlist[] = {"rt", "root", NULL};

    if(!PyArg_ParseTuple(args, "O", &rt))
    {
        return NULL;
    }
    
    if(rt == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "No root object provided.");
        return NULL;
    }

    f = PyObject_Type(rt);
    if(f != NULL)
    {
        PyObject_Print(f, stderr, 0);
        Py_DECREF(f);
    }
    
    if(!PyObject_TypeCheck(rt, &RuntimeType))
    {
        PyObject_Print(args, stderr, 0);
        PyErr_SetString(PyExc_TypeError, "Invalid Runtime object.");
        return NULL;
    }
   
    fprintf(stderr, "For real\n");
    
    if(!PyArg_ParseTupleAndKeywords(args, kwargs, "|O", kwlist, &root))
    {
        return NULL;
    }

    runtime = (Runtime*) rt;
    self = (Context*) type->tp_alloc(type, 0);
    if(self != NULL)
    {
        self->cx = JS_NewContext(runtime->rt, 8192);
        if(self->cx == NULL)
        {
            PyErr_SetString(PyExc_RuntimeError, "Failed to create JSContext.");
            Py_DECREF(self);
            return NULL;
        }

        self->root = PyString_FromString("");
        if(self->root == NULL)
        {
            Py_DECREF(self);
            return NULL;
        }
    }

    return (PyObject*) self;
}

int
Context_init(Context* self, PyObject* args, PyObject* kwargs)
{
    return 0;
}

void
Context_dealloc(Context* self)
{
    if(self->cx != NULL)
    {
        JS_DestroyContext(self->cx);
    }
    Py_XDECREF(self->root);
}
