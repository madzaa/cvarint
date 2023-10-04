#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject *cvarint_encode(PyObject *self, PyObject *args)
{
    unsigned long long n = 0;
    char out[10];

    if (!PyArg_ParseTuple(args, "K", &n))
    {
        return NULL;
    }

    int counter = 0;

    while (n > 0)
    {
        int part = n & 0x7f;
        n >>= 7;
        if (n > 0)
        {
            part |= 0x80;
        }
        out[counter] = part;
        counter++;
    }

    return Py_BuildValue("y#", out, counter);
}

static PyObject *cvarint_decode(PyObject *self, PyObject *args)
{

    const char *value;
    unsigned long long n = 0;

    if (!PyArg_ParseTuple(args, "y", &value))
    {
        return NULL;
    }

    int start = 0;
    int end = strlen(value) - 1;

    char reverseArray[strlen(value) + 1];

    while (start <= end)
    {
        char temp = value[start];
        reverseArray[start] = value[end];
        reverseArray[end] = temp;
        start++;
        end--;
    }
    reverseArray[strlen(value)] = '\0';

    for (size_t i = 0; i < strlen(reverseArray); i++)
    {
        n <<= 7;
        n |= (reverseArray[i] & 0x7f);
    }
    return Py_BuildValue("K", n);
}

static PyMethodDef CVarintMethods[] = {
    {"encode", cvarint_encode, METH_VARARGS, "Encode an integer as varint."},
    {"decode", cvarint_decode, METH_VARARGS, "Decode varint bytes to an integer."},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef cvarintmodule = {
    PyModuleDef_HEAD_INIT, "cvarint",
    "A C implementation of protobuf varint encoding", -1, CVarintMethods};

PyMODINIT_FUNC PyInit_cvarint(void) { return PyModule_Create(&cvarintmodule); }

// 1
// 150
// 18446744073709551615
