public class Dotproduct {
    public static double dotProduct(int[] a, int[] b) {
        double result = 0;
        for (int i = 0; i < a.length; i++) {
            result += (double)a[i] * b[i];
        }
        return result;
    }

    public static void main(String[] args) {
        int n = 1000; // Vector length, can be changed
        int[] a = new int[n];
        int[] b = new int[n];

        // Generate random vector values
        for (int i = 0; i < n; i++) {
            a[i] = (int) (Math.random() * Integer.MAX_VALUE);
            b[i] = (int) (Math.random() * Integer.MAX_VALUE);
        }

        long start = System.nanoTime();
        double result = dotProduct(a, b);
        long end = System.nanoTime();

        double time_spent = (end - start) / 1e9;

        System.out.println("Dot product: " + result);
        System.out.println("Time taken: " + time_spent + " seconds");
    }
}
