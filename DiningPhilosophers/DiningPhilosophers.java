//Author: Vijay
//no-deadlock version
import java.util.concurrent.Semaphore;

public class DiningPhilosophers {
  static int n = 5;
  public static final String[] Phils = {"A", "B", "C", "D", "E"};
  static Philosopher[] philosophers = new Philosopher[5];
  static Semaphore mutex = new Semaphore(1);

  public static void main(String[] args){
    philosophers[0] = new Philosopher(0);
    for (int i = 1; i < n; i++) {
      philosophers[i] = new Philosopher(i);
    }
    for(Thread threads : philosophers){
      threads.start();
    }
  }

  public static class Philosopher extends Thread{

    private enum State{STARTS_THINKING, HUNGRY, EATING};

    public final int id;
    protected State state;
    protected final Semaphore self;

    Philosopher(int id) {
      this.id = id;
      self = new Semaphore(0);
      state = State.STARTS_THINKING;
    }
    
    private Philosopher left() {
      return philosophers[id == 0?n - 1:id - 1];
    }

    private Philosopher right() {
      return philosophers[(id + 1) % n];
    }
    
    public void run(){
      try {
        while (true) {
          printState();
          switch(state) {
          case STARTS_THINKING: 
            action();
            mutex.acquire();
            state = State.HUNGRY; 
            break;
          case HUNGRY:
            test(this);
            mutex.release();
            self.acquire();
            state = State.EATING;
            break;
          case EATING:
            action();
            mutex.acquire();
            state = State.STARTS_THINKING;
            test(left());  
            test(right());
            mutex.release();
            break;          
          }
        }
      } catch(InterruptedException e) {}
    }

    static private void test(Philosopher p) { //eat only if both left and right forks are acquired
      if (p.left().state != State.EATING && p.state == State.HUNGRY &&
          p.right().state != State.EATING) {
        System.out.println("Philosopher " + Phils[p.id] + " grabs fork " + (p.id - 1)%5 + " (left) and fork " + (p.id + 1)%5+ " (right)");
        p.state = State.EATING;
        p.self.release();
      }
    }

    private void action() throws InterruptedException {
        Thread.sleep((long) Math.round(Math.random() * 2000)); //2000 corresponds to 2 seconds
    }

    private void printState() {
      System.out.println("Philosopher " + Phils[id] + " is " + state);
    }
  }
}