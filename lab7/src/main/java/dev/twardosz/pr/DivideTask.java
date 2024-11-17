package dev.twardosz.pr;

import java.util.Arrays;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveTask;

class DivideTask extends RecursiveTask<int[]> {

    int[] arrayToDivide;

    public DivideTask(int[] arrayToDivide) {
        this.arrayToDivide = arrayToDivide;
    }

    protected int[] compute() {
        if (arrayToDivide.length <= 1) {
            return arrayToDivide;
        }

        // .......

        int mid = arrayToDivide.length / 2;
        int[] leftPart = new int[mid];
        int[] rightPart = new int[arrayToDivide.length - mid];

        System.arraycopy(arrayToDivide, 0, leftPart, 0, mid);
        System.arraycopy(arrayToDivide, mid, rightPart, 0, arrayToDivide.length - mid);

        // Tworzenie nowych zadań dla podtablic
        DivideTask task1 = new DivideTask(leftPart);
        DivideTask task2 = new DivideTask(rightPart);

        invokeAll(task1, task2);

        //Wait for results from both tasks
        int[] tab1 = task1.join();
        int[] tab2 = task2.join();

        int[] scal_tab = new int[tab1.length + tab2.length];
        scal_tab(tab1, tab2, scal_tab);

        return scal_tab;
    }

    private void scal_tab(
            int[] tab1,
            int[] tab2,
            int[] scal_tab) {

        int i = 0, j = 0, k = 0;

        while ((i < tab1.length) && (j < tab2.length)) {

            if (tab1[i] < tab2[j]) {
                scal_tab[k] = tab1[i++];
            } else {
                scal_tab[k] = tab2[j++];
            }

            k++;
        }

        if (i == tab1.length) {

            for (int a = j; a < tab2.length; a++) {
                scal_tab[k++] = tab2[a];
            }

        } else {

            for (int a = i; a < tab1.length; a++) {
                scal_tab[k++] = tab1[a];
            }

        }
    }

    public static void main(String[] args) {
        // Testowa tablica
        int[] array = {38, 27, 43, 3, 9, 82, 10};
        System.out.println("Przed sortowaniem: " + Arrays.toString(array));

        // Utworzenie ForkJoinPool
        ForkJoinPool pool = new ForkJoinPool();

        // Rozpoczęcie zadania
        DivideTask mainTask = new DivideTask(array);
        int[] sortedArray = pool.invoke(mainTask);

        System.out.println("Po sortowaniu: " + Arrays.toString(sortedArray));
    }
}
