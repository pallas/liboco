#ifndef KILLABLE_H
#define KILLABLE_H

class killable {
public:
  killable();

  void kill();
  bool dead() const;

protected:
  virtual void die();

private:
  bool dead_;
};

#endif//KILLABLE_H
