#include <directfb/dfb_types.h>
#include <directfb/directfb.h>
IDirectFB  *dfb;
IDirectFBSurface *primary;
IDirectFBImageProvider *provider;
DFBSurfaceDescription dsc;

int main(int argc, char *argv[])
{
        DirectFBInit(&argc, &argv);
        DirectFBCreate(&dfb);

        dfb->SetCooperativeLevel(dfb, DFSCL_FULLSCREEN);
        memset(&dsc, 0, sizeof(DFBSurfaceDescription));
        dsc.flags = DSDESC_CAPS;;
        dsc.caps = DSCAPS_PRIMARY;
        dfb->CreateSurface(dfb, &dsc, &primary);
        dfb->CreateImageProvider(dfb, "dfbTest.jpg", &provider);
        provider->RenderTo(provider, primary, (void*)0);
        provider->Release(provider);
        sleep(5);
        primary->Release(primary);
        dfb->Release(dfb);
}
