from cffi import FFI

ffi = FFI()




if __name__ == "__main__":
    filename  = sys.argv[1]
    parse(filename)
