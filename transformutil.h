#ifndef TRANSFORMUTIL_H
#define TRANSFORMUTIL_H


namespace Util {
//Found here: http://stackoverflow.com/questions/11459318/maintaining-relative-child-position-after-applying-qgraphicsitemitemignorestra
static QTransform GenerateTranslationOnlyTransform(
        const QTransform &original_transform,
        const QPointF &target_point) {
    // To draw the unscaled icons, we desire a transform with scaling factors
    // of 1 and shearing factors of 0 and the appropriate translation such that
    // our icon center ends up at the same point. According to the
    // documentation, QTransform transforms a point in the plane to another
    // point using the following formulas:
    // x' = m11*x + m21*y + dx
    // y' = m22*y + m12*x + dy
    //
    // For our new transform, m11 and m22 (scaling) are 1, and m21 and m12
    // (shearing) are 0. Since we want x' and y' to be the same, we have the
    // following equations:
    // m11*x + m21*y + dx = x + dx[new]
    // m22*y + m12*x + dy = y + dy[new]
    //
    // Thus,
    // dx[new] = m11*x - x + m21*y + dx
    // dy[new] = m22*y - y + m12*x + dy
    qreal dx = original_transform.m11() * target_point.x()
            - target_point.x()
            + original_transform.m21() * target_point.y()
            + original_transform.m31();
    qreal dy = original_transform.m22() * target_point.y()
            - target_point.y()
            + original_transform.m12() * target_point.x()
            + original_transform.m32();

    return QTransform::fromTranslate(dx, dy);
}
}


#endif // TRANSFORMUTIL_H
