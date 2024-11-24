package dev.twardosz.pr.bariera;

public class TestBariera {
    public static void main(String[] args) {
        Bariera bariera = new Bariera(3); // Bariera dla 3 wątków

        Runnable zadanie = () -> {
            try {
                System.out.println(Thread.currentThread().getName() + " czeka na barierze...");
                bariera.czekaj();
                System.out.println(Thread.currentThread().getName() + " przekroczył barierę!");
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        };

        // Tworzenie wątków
        for (int i = 0; i < 6; i++) {
            new Thread(zadanie, "Watek-" + i).start();
        }
    }
}
