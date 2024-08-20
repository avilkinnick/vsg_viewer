#ifndef GUI_H
#define GUI_H

#include <vsg/all.h>

class GUI : public vsg::Inherit<vsg::Command, GUI>
{
public:
    void record(vsg::CommandBuffer& cb) const override;
};

#endif // GUI_H
