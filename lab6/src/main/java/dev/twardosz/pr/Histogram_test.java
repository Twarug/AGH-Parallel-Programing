package dev.twardosz.pr;

import java.util.Scanner;


class Histogram_test {

    public static void main(String[] args) {

        Scanner scanner = new Scanner(System.in);

//        System.out.println("Set image size: n (#rows), m(#kolumns)");
        int n = 100; //scanner.nextInt();
        int m = 100; // scanner.nextInt();
        Obraz obraz_0 = new Obraz(n, m);

        obraz_0.calculate_histogram();
//        obraz_0.print_histogram();

        // System.out.println("Set number of threads");
         int num_threads = 16; //scanner.nextInt();

        Obraz obraz_1 = parallel1(obraz_0);
//         obraz_1.visualize_histogram();

        Obraz obraz_2 = parallel2(obraz_0, num_threads);
//         obraz_2.visualize_histogram();

        Obraz obraz_3 = parallel3(obraz_0, num_threads);
//         obraz_3.visualize_histogram();

        Obraz obraz_4 = parallel4(obraz_0, num_threads);
//         obraz_3.visualize_histogram();

        obraz_0.compare_histograms(obraz_1);
        obraz_0.compare_histograms(obraz_2);
        obraz_0.compare_histograms(obraz_3);
        obraz_0.compare_histograms(obraz_4);
    }

    static Obraz parallel1(Obraz obraz0) {
        var threads = new Thread[94];
        Obraz obraz = new Obraz(obraz0);

        for (int i = 0; i < 94; i++) {
            (threads[i] = new Watek1(obraz, (char)(i+33))).start();
        }

        await(threads);

        return obraz;
    }

    static Obraz parallel2(Obraz obraz0, int num_threads) {
        var threads = new Thread[num_threads];
        Obraz obraz = new Obraz(obraz0);

        int symbolCount = 94;
        int perThread = symbolCount / num_threads;
        int remainder = symbolCount % num_threads;


        for (int i = 0; i < num_threads; i++) {
            char symbolStart = (char)(i * perThread);
            char symbolEnd = (char)(symbolStart + perThread + (i == num_threads - 1 ? remainder : 0));

            (threads[i] = new Thread(() -> obraz.calculate_histogram_parallel2(symbolStart, symbolEnd))).start();
        }

        await(threads);

        return obraz;
    }

    static Obraz parallel3(Obraz obraz0, int num_threads) {
        var threads = new Thread[num_threads];
        Obraz obraz = new Obraz(obraz0);

        int m = obraz.size(1);
        int perThread = obraz.size(0) / num_threads;
        int remainder = obraz.size(0) % num_threads;

        for (int i = 0; i < num_threads; i++) {
            int start = i * perThread;
            int end = start + perThread + (i == num_threads - 1 ? remainder : 0);

            (threads[i] = new Thread(() -> obraz.calculate_histogram_parallel3(start, end, 1, 0, m, 1))).start();
        }

        await(threads);

        return obraz;
    }

    static Obraz parallel4(Obraz obraz0, int num_threads) {
        var threads = new Thread[num_threads];
        Obraz obraz = new Obraz(obraz0);

        int n = obraz.size(0);
        int m = obraz.size(1);

        for (int i = 0; i < num_threads; i++) {
            int start = i;
            int stride = num_threads;
            int end = n;

            (threads[i] = new Thread(() -> obraz.calculate_histogram_parallel3(start, end, stride, 0, m, 1))).start();
        }

        await(threads);

        return obraz;
    }

    private static void await(Thread[] threads) {
        for (Thread t : threads) {
            try {
                t.join();
            } catch (InterruptedException ignored) {}
        }
    }
}
