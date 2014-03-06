#ifndef FILE_DESCRIPTOR_H
#define FILE_DESCRIPTOR_H

class file_descriptor {
public:
  file_descriptor();
  explicit file_descriptor(int); /// takes ownership
  file_descriptor(const file_descriptor &); /// dups
  ~file_descriptor();

  file_descriptor & operator=(file_descriptor);

  bool valid() const;

  int release();
  file_descriptor & capture(int);

  operator int () const;

  file_descriptor & block();
  file_descriptor & unblock();

  file_descriptor & open_on_exec();
  file_descriptor & close_on_exec();

  static int dup(int);

  static bool is_fifo(int);
  static bool is_socket(int);

  static int send_buffer(int);
  static int receive_buffer(int);

  static void keepalive(int);
  static void reuseaddr(int);

private:
  int fd;
};

#endif//FILE_DESCRIPTOR_H
