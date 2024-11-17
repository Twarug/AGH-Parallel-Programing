package dev.twardosz.pr;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class Calka_executor_test {

	private static final int NTHREADS = 4; // Liczba wątków
	private static final int NTASKS = 10; // Liczba podprzedziałów
	private static final double DX = 0.001; // Dokładność całkowania
	private static final double START = 0.0; // Początek przedziału
	private static final double END = Math.PI; // Koniec przedziału

    public static void main(String[] args) {
		ExecutorService executor = Executors.newFixedThreadPool(NTHREADS);
		List<Future<Double>> results = new ArrayList<>();

		double range = (END - START) / NTASKS;

		for (int i = 0; i < NTASKS; i++) {
			double start = START + i * range;
			double end = start + range;
			Callable<Double> task = new Calka_callable(start, end, DX);
			results.add(executor.submit(task));
		}

		// This will make the executor accept no new threads
		// and finish all existing threads in the queue
		executor.shutdown();

		// Odbieranie wyników
		double totalIntegral = 0.0;
		try {
			for (Future<Double> result : results) {
				totalIntegral += result.get(); // Blokuje do ukończenia danego zadania
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		System.out.println("Wynik całki: " + totalIntegral);
    }
}
