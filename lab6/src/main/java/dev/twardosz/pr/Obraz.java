package dev.twardosz.pr;

import  java.util.Random;


class Obraz {

    private final int size_n;
    private final int size_m;
    private final char[][] tab;
    private final char[] tab_symb;
    private final int[] histogram;

    public Obraz(int n, int m) {

        this.size_n = n;
        this.size_m = m;
        tab = new char[n][m];
        tab_symb = new char[94];

        final Random random = new Random();

        // for general case where symbols could be not just integers
        for(int k=0;k<94;k++) {
            tab_symb[k] = (char)(k+33); // substitute symbols
        }

        for(int i=0;i<n;i++) {
            for(int j=0;j<m;j++) {
                tab[i][j] = tab_symb[random.nextInt(94)];  // ascii 33-127
                //tab[i][j] = (char)(random.nextInt(94)+33);  // ascii 33-127
//                System.out.print(tab[i][j]+" ");
            }
//            System.out.print("\n");
        }
//        System.out.print("\n\n");

        histogram = new int[94];
        clear_histogram();
    }

    public Obraz(Obraz other) {
        this.size_n = other.size_n;
        this.size_m = other.size_m;
        this.tab = other.tab;
        this.tab_symb = other.tab_symb;
        this.histogram = new int[94];
        clear_histogram();
    }

    public void clear_histogram(){

        for(int i=0;i<94;i++) histogram[i]=0;

    }

    public void calculate_histogram(){

        for(int i=0;i<size_n;i++) {
            for(int j=0;j<size_m;j++) {
                for(int k=0;k<94;k++) {
                    if(tab[i][j] == tab_symb[k]) histogram[k]++;
                    //if(tab[i][j] == (char)(k+33)) histogram[k]++;
                }

            }
        }

    }

    public void print_histogram() {
        for(int i = 0; i < 94; i++)
            System.out.print(tab_symb[i] + " " + histogram[i] + "\n");
    }

    public void visualize_histogram() {
        for (char s : tab_symb) {
            System.out.print(s + " ");
            for (int i = 0; i < histogram[s - 33]; i++) {
                System.out.print("=");
            }
            System.out.println();
        }
    }

    public void compare_histograms(Obraz other){
        boolean histograms_are_the_same = true;
        for(int i=0;i<94;i++) {
            if(histogram[i] != other.histogram[i]) {
                histograms_are_the_same = false;
                System.out.print(tab_symb[i]+" "+histogram[i]+" "+other.histogram[i]+"\n");
            }
        }

        if (histograms_are_the_same) System.out.println("Histograms are the same.");
        else System.out.println("Histograms are different.");
    }

    public void calculate_histogram_parallel1(char symbol) {
        for(int i=0;i<size_n;i++) {
            for(int j=0;j<size_m;j++) {
                if(tab[i][j] == symbol)
                    synchronized(histogram) {
                        histogram[symbol - 33]++;
                    }
            }
        }
    }

    public void calculate_histogram_parallel2(char symbolStart, char symbolEnd) {
        for(int x = 0; x < size_n; x++)
            for (int y = 0; y < size_m; y++)
                for(int k = symbolStart; k < symbolEnd; k++)
                    if (tab[x][y] == tab_symb[k])
                        synchronized (histogram) {
                            histogram[k]++;
                        }
    }

    public void calculate_histogram_parallel3(int x1, int x2, int dx, int y1, int y2, int dy) {
        for (int x = x1; x < x2; x += dx)
            for (int y = y1; y < y2; y += dy)
                for(int k = 0; k < 94; k++)
                    if (tab[x][y] == tab_symb[k])
                        synchronized (histogram) {
                            histogram[k]++;
                        }
    }

    public int size(int i) {
        return switch (i) {
            case 0 -> size_n;
            case 1 -> size_m;
            default -> throw new IllegalArgumentException();
        };
    }
}
