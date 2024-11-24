package dev.twardosz.pr.bariera;

class Bariera {
    private final int liczbaWymagana;
    private int liczbaOczekujacych = 0;

    public Bariera(int liczbaWymagana) {
        this.liczbaWymagana = liczbaWymagana;
    }

    public synchronized void czekaj() throws InterruptedException {
        liczbaOczekujacych++;
        if (liczbaOczekujacych < liczbaWymagana) {
            wait(); // Czekanie na spełnienie warunku
        } else {
            liczbaOczekujacych = 0; // Reset bariery
            notifyAll(); // Obudzenie wszystkich
        }
    }
}
