package dev.twardosz.pr.czytelnia;

public class TestCzytelnia {
    public static void main(String[] args) {
        Czytelnia czytelnia = new Czytelnia();

        // Tworzenie wątków Czytelników
        for (int i = 0; i < 5; i++) {
            new Thread(() -> {
                try {
                    while (true) {
                        czytelnia.chceCzytac();
                        czytelnia.czytam();
                        czytelnia.koniecCzytania();
                    }
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }, "Czytelnik-" + i).start();
        }

        // Tworzenie wątków Pisarzy
        for (int i = 0; i < 2; i++) {
            new Thread(() -> {
                try {
                    while (true) {
                        czytelnia.chcePisac();
                        czytelnia.pisze();
                        czytelnia.koniecPisania();
                    }
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }, "Pisarz-" + i).start();
        }
    }
}
