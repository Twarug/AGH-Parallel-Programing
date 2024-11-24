package dev.twardosz.pr.czytelnia;

import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class Czytelnia {
    private int liczbaCzytelnikow = 0; // Liczba aktywnych czytelników
    private int liczbaPisarzy = 0;    // Liczba aktywnych pisarzy
    private final ReentrantLock lock = new ReentrantLock();
    private final Condition czytelnicyCondition = lock.newCondition();
    private final Condition pisarzeCondition = lock.newCondition();

    // Czytelnik chce czytać
    public void chceCzytac() throws InterruptedException {
        lock.lock();
        try {
            while (liczbaPisarzy > 0) { // Czeka na brak pisarzy
                czytelnicyCondition.await();
            }
            liczbaCzytelnikow++;
        } finally {
            lock.unlock();
        }
    }

    // Czytelnik kończy czytanie
    public void koniecCzytania() {
        lock.lock();
        try {
            liczbaCzytelnikow--;
            if (liczbaCzytelnikow == 0) { // Budzi pisarzy, jeśli nie ma aktywnych czytelników
                pisarzeCondition.signal();
            }
        } finally {
            lock.unlock();
        }
    }

    // Pisarz chce pisać
    public void chcePisac() throws InterruptedException {
        lock.lock();
        try {
            while (liczbaCzytelnikow > 0 || liczbaPisarzy > 0) { // Czeka na brak czytelników i pisarzy
                pisarzeCondition.await();
            }
            liczbaPisarzy++;
        } finally {
            lock.unlock();
        }
    }

    // Pisarz kończy pisanie
    public void koniecPisania() {
        lock.lock();
        try {
            liczbaPisarzy--;
            if (lock.hasWaiters(czytelnicyCondition)) { // Jeśli są czekający czytelnicy, budzi ich
                czytelnicyCondition.signalAll();
            } else { // W przeciwnym razie budzi pisarzy
                pisarzeCondition.signal();
            }
        } finally {
            lock.unlock();
        }
    }

    // Metody pomocnicze (symulacja)
    public void czytam() throws InterruptedException {
        System.out.println(Thread.currentThread().getName() + " czyta...");
        Thread.sleep((long) (Math.random() * 3000));
    }

    public void pisze() throws InterruptedException {
        System.out.println(Thread.currentThread().getName() + " pisze...");
        Thread.sleep((long) (Math.random() * 3000));
    }

}
