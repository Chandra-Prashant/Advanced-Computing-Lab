import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.util.List;

class Point {
    int x, y;
    Point(int x, int y) { this.x = x; this.y = y; }
}

public class Jarvis extends JPanel {
    private final List<Point> points = new ArrayList<>();
    private final Stack<Point> stack = new Stack<>();
    private Point current = null;
    private boolean started = false, finished = false;

    public Jarvis() {
        setBackground(Color.WHITE);
        addMouseListener(new MouseAdapter() {
            public void mouseClicked(MouseEvent e) {
                if (started) resetAlgorithm();
                points.add(new Point(e.getX(), e.getY()));
                repaint();
            }
        });
    }
    // Orientation: 0=collinear, 1=CW, 2=CCW
    static int orientation(Point p, Point q, Point r) {
        int val = (q.y - p.y) * (r.x - q.x) -
                  (q.x - p.x) * (r.y - q.y);
        if (val == 0) return 0;
        return (val > 0) ? 1 : 2;
    }
    static int distSq(Point a, Point b) {
        return (a.x - b.x)*(a.x - b.x) +
               (a.y - b.y)*(a.y - b.y);
    }
    // ---------------- JARVIS START ----------------
    public void startAlgorithm() {
        if (points.size() < 3) return;
        resetAlgorithm();
        started = true;
        // Find leftmost point
        Point leftMost = Collections.min(points,
                Comparator.comparingInt((Point p) -> p.x)
                          .thenComparingInt(p -> p.y));
        stack.push(leftMost);
        current = leftMost;
        repaint();
    }
    public void nextStep() {
        if (!started || finished) return;
        Point last = stack.peek();
        Point nextPoint = null;
        for (Point p : points) {
            if (p == last) continue;
            if (nextPoint == null)
                nextPoint = p;
            int o = orientation(last, nextPoint, p);
            // Pick most counter-clockwise point
            if (o == 2 || (o == 0 && distSq(last, p) > distSq(last, nextPoint))) {
                nextPoint = p;
            }
        }
        // If wrapped back to start → finished
        if (nextPoint == stack.firstElement()) {
            finished = true;
            current = null;
            repaint();
            return;
        }
        stack.push(nextPoint);
        current = nextPoint;
        repaint();
    }
    // ---------------- RESET ----------------
    private void resetAlgorithm() {
        stack.clear();
        current = null;
        started = false;
        finished = false;
    }
    public void clearAll() {
        points.clear();
        resetAlgorithm();
        repaint();
    }
    // ---------------- DRAWING ----------------
    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        for (Point p : points) {
            if (p == current) g.setColor(Color.BLUE);
            else if (stack.contains(p)) g.setColor(Color.GREEN);
            else g.setColor(Color.BLACK);
            g.fillOval(p.x - 5, p.y - 5, 10, 10);
            g.setColor(Color.GRAY);
            g.drawString("(" + p.x + "," + p.y + ")", p.x + 5, p.y - 5);
        }
        if (stack.size() > 1) {
            g.setColor(Color.RED);
            for (int i = 0; i < stack.size() - 1; i++) {
                Point a = stack.get(i);
                Point b = stack.get(i + 1);
                g.drawLine(a.x, a.y, b.x, b.y);
            }
            if (finished) {
                Point first = stack.firstElement();
                Point last = stack.peek();
                g.drawLine(first.x, first.y, last.x, last.y);
            }
        }
    }

    public static void main(String[] args) {
        JFrame frame = new JFrame("Convex Hull - Jarvis March (Step by Step)");
        Jarvis panel = new Jarvis();
        JButton start = new JButton("Start");
        JButton next = new JButton("Next Step");
        JButton clear = new JButton("Clear");
        start.addActionListener(e -> panel.startAlgorithm());
        next.addActionListener(e -> panel.nextStep());
        clear.addActionListener(e -> panel.clearAll());
        JPanel controls = new JPanel();
        controls.add(start);
        controls.add(next);
        controls.add(clear);
        frame.setLayout(new BorderLayout());
        frame.add(panel, BorderLayout.CENTER);
        frame.add(controls, BorderLayout.SOUTH);
        frame.setSize(900, 650);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);
    }
}
