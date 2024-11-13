package dev.twardosz.pr;

public class Watek1 extends Thread {

    private final Obraz obraz;
    private final char symbol;

    public Watek1(Obraz obraz, char symbol) {
        this.obraz = obraz;
        this.symbol = symbol;
    }

    public void run() {
        obraz.calculate_histogram_parallel1(symbol);
    }
}
