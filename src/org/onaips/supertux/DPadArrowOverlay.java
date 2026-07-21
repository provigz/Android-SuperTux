package org.onaips.supertux;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.view.View;

public class DPadArrowOverlay extends View
{
    private Paint paint;
    private Path path;

    public DPadArrowOverlay(Context context)
    {
        super(context);
        paint = new Paint();
        paint.setColor(Color.argb(200, 255, 255, 255)); // Translucent White
        paint.setStyle(Paint.Style.FILL);
        paint.setAntiAlias(true);
        path = new Path();
    }

    @Override
    protected void onDraw(Canvas canvas)
    {
        super.onDraw(canvas);
        int w = getWidth();
        int h = getHeight();
        int cellW = w / 3;
        int cellH = h / 3;
        int arrowSize = Math.min(cellW, cellH) / 4;

        drawTriangle(canvas, cellW + cellW / 2, cellH / 2, arrowSize, 0); // Up arrow
        drawTriangle(canvas, cellW + cellW / 2, cellH * 2 + cellH / 2, arrowSize, 180); // Down arrow
        drawTriangle(canvas, cellW / 2, cellH + cellH / 2, arrowSize, 270); // Left arrow
        drawTriangle(canvas, cellW * 2 + cellW / 2, cellH + cellH / 2, arrowSize, 90); // Right arrow
    }

    private void drawTriangle(Canvas canvas, int cx, int cy, int size, float angle)
    {
        canvas.save();
        canvas.rotate(angle, cx, cy);

        path.reset();
        path.moveTo(cx, cy - size);
        path.lineTo(cx - size, cy + size);
        path.lineTo(cx + size, cy + size);
        path.close();

        canvas.drawPath(path, paint);
        canvas.restore();
    }
}
