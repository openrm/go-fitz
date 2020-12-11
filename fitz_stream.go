package fitz

/*
#include <mupdf/fitz.h>
*/
import "C"

import (
    "io"
    "sync"
    "unsafe"
)

type registry struct {
    mu sync.Mutex
    index int
    store map[int]interface{}
}

var reg = &registry{
    store: make(map[int]interface{}),
}

func (r *registry) register(stm interface{}) int {
    r.mu.Lock()
    defer r.mu.Unlock()
    r.index++
    for r.store[r.index] != nil {
        r.index++
    }
    r.store[r.index] = stm
    return r.index
}

func (r *registry) lookup(i int) interface{} {
 r.mu.Lock()
 defer r.mu.Unlock()
 return r.store[i]
}

func (r *registry) unregister(i int) {
 r.mu.Lock()
 defer r.mu.Unlock()
 delete(r.store, i)
}

//export readBytes
func readBytes(key C.int, ptr *C.uchar, max C.int) C.int {
    reader := reg.lookup(int(key)).(io.Reader)
    buf := (*[4096]byte)(unsafe.Pointer(ptr))[:]
    n, err := reader.Read(buf)
    *ptr = *(*C.uchar)(unsafe.Pointer(&buf[0]))
    if err == io.EOF {
        return 0
    } else if err != nil {
        return -1
    }
    return C.int(n)
}
